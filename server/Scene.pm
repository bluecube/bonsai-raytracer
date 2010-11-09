use strict;
use warnings;

package Scene;

use Data::Dumper;
use JSON::XS;
use Math::MatrixReal;

use Raw;

use constant PI => 4 * atan2(1, 1);

# Our default camera uses full-frame sensor
# (36mm x 24 mm), with 50mm focal length and f/4.0
use constant {
	DEFAULT_SENSOR_WIDTH => 36e-3,
	DEFAULT_SENSOR_HEIGHT => 24e-3,
};
use constant DEFAULT_ASPECT => DEFAULT_SENSOR_WIDTH / DEFAULT_SENSOR_HEIGHT;
use constant DEFAULT_FOCAL_LENGTH => 50e-3;
use constant DEFAULT_F_NUMBER => 4.0;
use constant DEFAULT_FOCUS_DISTANCE => 200;

# Default number of rays sent through each pixel.
use constant DEFAULT_RAYS_PER_PX => 1e3;

# object types and their handlers:
my %objectTypes = (
	polygonal => sub{
		my $ret = Raw::load($_->{'file'});

		die $ret unless ref($ret);

		delete $_->{'file'};
		$_->{'vertices'} = $ret->{'vertices'};
		$_->{'polygons'} = $ret->{'polygons'};
	},

	sphere => sub{
		my $r = 1;
		$r = $_->{'r'} if defined($_->{'r'});

		my $center = [0, 0, 0];
		$center = $_->{'center'} if defined($_->{'center'});

		push @$center, 1;

		Math::MatrixReal->new_from_rows([
			[$r, 0, 0, 0],
			[0, $r, 0, 0],
			[0, 0, $r, 0],
			$center
			]);
	},

	plane => sub{
		my $normal = [0, 1, 0];
		$normal = $_->{'normal'} if defined($_->{'normal'});
		push @$normal, 0;
		$normal = Math::MatrixReal->new_from_rows([$normal]);
		my $len = vector_length($normal);
		die "Plane normal vector can't be zero" if $len == 0;
		$normal /= $len;

		#now we need to select the first two rows of the matrix.
		#these will be mapped to vectors (1, 0, 0) and (0, 0, 1) in object space.
		#the first vector is obtained by taking (1, 0, 0) and projecting it onto
		#the plane. If the normal is too close to this vector, then (0, 0, 1) is
		#taken instead
		#the second vector is taken as a cross product between the first vector
		#and the normal.
		#third row of the matrix is the plane normal and fourth is the translation
		#of the origin

		my $basex = Math::MatrixReal->new_from_rows([[1, 0, 0, 0]]);
		my $dotProduct = dot_product($normal, $basex);
		if(abs($dotProduct) > cos(PI / 6)){ # this constant is pertty much arbitrary. I only need the base vector reasonably far from the normal vector.
			$basex = Math::MatrixReal->new_from_rows([[0, 0, 1, 0]]);
			$dotProduct = dot_product($normal, $basex);
		}
		$basex -= $dotProduct * $normal; #this is the projection

		# normalize the projected vector
		$basex /= vector_length($basex);

		my $basey = vector_product($basex, $normal);

		my $point = [0, 0, 0];
		$point = $_->{'point'} if defined($_->{'point'});
		push @$point, 1;

		Math::MatrixReal->new_from_rows([
			$basex,
			$basey,
			$normal,
			$point
			]);
	},

	CSG => sub{
		die "Operation must be specified" unless defined $_->{'operation'};
		objects();
	}
);

# return an array ref representing the given matrix in the protocol
sub writeMatrix{
	my $m = shift;

	return [
		$m->element(1, 1) + 0,
		$m->element(1, 2) + 0,
		$m->element(1, 3) + 0,
		$m->element(2, 1) + 0,
		$m->element(2, 2) + 0,
		$m->element(2, 3) + 0,
		$m->element(3, 1) + 0,
		$m->element(3, 2) + 0,
		$m->element(3, 3) + 0,
		$m->element(4, 1) + 0,
		$m->element(4, 2) + 0,
		$m->element(4, 3) + 0
	]
}

sub readMatrix{
	my $t = shift;

	die "4x4 matrix (array of 16 numbers) expected" unless
		ref($t) eq 'ARRAY' && @$t == 16;

	$t = Math::MatrixReal->new_from_rows([
			[@{$t}[0 .. 3]],
			[@{$t}[4 .. 7]],
			[@{$t}[8 .. 11]],
			[@{$t}[12 .. 15]]]);

	return $t;
}

# Vector product, working around some limitations of Math::MatrixReal
sub vector_product{
	my $a = shift;
	my $b = shift;

	Math::MatrixReal->new_from_rows([[
			$a->element(1, 2) * $b->element(1, 3) - $a->element(1, 3) * $b->element(1, 2),
			$a->element(1, 3) * $b->element(1, 1) - $a->element(1, 1) * $b->element(1, 3),
			$a->element(1, 1) * $b->element(1, 2) - $a->element(1, 2) * $b->element(1, 1),
			0
			]]);
}

sub dot_product{
	my $a = shift;
	my $b = shift;

	(~$a)->scalar_product(~$b);
}

sub vector_length{
	my $a = shift;

	(~$a)->length;
}

# process camera settings and normalize its dimensions.
# The real sensor is always 1 unit wide and (1 / aspect) high.
sub cameraSettings{
	$_->{'camera'} = {} unless defined($_->{'camera'});
	local $_ = $_->{'camera'};

	# focal length
	$_->{'focalLength'} = DEFAULT_FOCAL_LENGTH unless defined($_->{'focalLength'});

	# sensor size
	# the camera is always scaled to have the sensor width == 1
	if(!defined($_->{'sensorSize'})){
		$_->{'aspect'} = DEFAULT_ASPECT;
		$_->{'sensorWidth'} = DEFAULT_SENSOR_WIDTH;
	}elsif(ref($_->{'sensorSize'}) eq 'SCALAR'){
		$_->{'aspect'} = DEFAULT_ASPECT;
		$_->{'sensorWidth'} = $_->{'sensorSize'} + 0;
	}elsif(ref($_->{'sensorSize'}) eq 'ARRAY'){
		$_->{'aspect'} = $_->{'sensorSize'}->[0] / $_->{'sensorSize'}->[1];
		$_->{'sensorWidth'} = $_->{'sensorSize'}->[0];
	}
	delete $_->{'sensorSize'};

	# f-number and aperture
	if(defined($_->{'fNumber'})){
		$_->{'apertureDiameter'} = 
			(($_->{'fNumber'} eq 'inf') ?
			0 :
			(1 / $_->{'fNumber'}));
	}else{
		$_->{'apertureDiameter'} = 1 / DEFAULT_F_NUMBER;
	}
	delete $_->{'fNumber'};

	if(!defined($_->{'raysPerPx'})){
		$_->{'raysPerPx'} = DEFAULT_RAYS_PER_PX;
	}elsif($_->{'raysPerPx'} <= 0){
		$_->{'raysPerPx'} = 1;
	}else{
		$_->{'raysPerPx'} = $_->{'raysPerPx'} + 0;
	}

	# transformation matrix

	my $position;
	if(defined($_->{'position'})){
		push @{$_->{'position'}}, 1;
		$position = Math::MatrixReal->new_from_rows([$_->{'position'}]);
		delete $_->{'position'};
	}else{
		$position = Math::MatrixReal->new_from_rows([[0, 0, 0, 1]]);
	}

	my $lookAt;
	if(defined($_->{'lookAt'})){
		push @{$_->{'lookAt'}}, 1;
		$lookAt = Math::MatrixReal->new_from_rows([$_->{'lookAt'}]);
		delete $_->{'lookAt'};
	}else{
		$lookAt = Math::MatrixReal->new_from_rows([[0, 0, DEFAULT_FOCUS_DISTANCE, 0]]);
	}

	my $upVector;
	if(defined($_->{'upVector'})){
		push @{$_->{'upVector'}}, 0;
		$upVector = Math::MatrixReal->new_from_rows([$_->{'upVector'}]);
		delete $_->{'upVector'};
	}else{
		$upVector = Math::MatrixReal->new_from_rows([[0, 1, 0, 0]]);
	}

	#print "upVector:";
	#print $upVector;
	#print "lookAt:";
	#print $lookAt;
	#print "position:";
	#print $position;

	my $zAxis = $lookAt - $position;
	$_->{'focus'} = vector_length($zAxis);
	$zAxis /= vector_length($zAxis);
	#print "zaxis:";
	#print $zAxis;

	my $xAxis = vector_product($upVector, $zAxis);
	$xAxis /= vector_length($xAxis);
	#print "xaxis:";
	#print $xAxis;

	my $yAxis = vector_product($zAxis, $xAxis);
	#print "yaxis:";
	#print $yAxis;

	my $matrix = Math::MatrixReal->new_from_rows([
		$xAxis,
		$yAxis,
		$zAxis,
		$position]);

	#print "\ncamera inverse transform:\n";
	#print $matrix;
	$_->{'transform'} = $matrix->inverse();
}

sub objects{
	my $cameraTransform = shift;
	$_->{'objects'} = [] unless defined($_->{'objects'}) &&
		(ref($_->{'objects'}) eq 'ARRAY');
	local $_ = $_->{'objects'};

	object($cameraTransform) for (@$_);
}

sub object{
	my $cameraTransform = shift;
	my $type = $_->{'type'};
	
	die("Object type must be specified") unless defined $type;
	
	my $transform;
	if(defined($_->{'transform'})){
		$transform = readMatrix($_->{'transform'});
	}else{
		$transform = Math::MatrixReal->new_diag([1, 1, 1, 1]); 
	}
	
	my $newTransform = &{$objectTypes{$type}}() if defined($objectTypes{$type});
	#print "Object: $type\n";
	
	if($newTransform){
		$transform = $newTransform * $transform;
	}

	#print "\n$type transform:\n";
	#print $transform;

	$transform *= $cameraTransform;
	
	$_->{'transform'} = writeMatrix($transform);
}


sub load{
	my $filename = shift;
	
	local $/;
	undef $/;

	open(my $fh, '<', $filename) or die $!;

	local $_ = decode_json <$fh>;

	close $fh;

	cameraSettings();
	objects($_->{'camera'}->{'transform'});

	delete $_->{'camera'}->{'transform'};
	$_;
}

1;
