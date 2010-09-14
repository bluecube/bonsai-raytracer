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

		push @$center, 0;

		Math::MatrixReal->new_from_rows( [
				[$r, 0, 0, 0],
				[0, $r, 0, 0],
				[0, 0, $r, 0],
				$center
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
		$position = Math::MatrixReal->new_from_cols([$_->{'position'}]);
		delete $_->{'position'};
	}else{
		$position = Math::MatrixReal->new_from_cols([[0, 0, 0]]);
	}

	my $lookAt;
	if(defined($_->{'lookAt'})){
		$lookAt = Math::MatrixReal->new_from_cols([$_->{'lookAt'}]);
		delete $_->{'lookAt'};
	}else{
		$lookAt = Math::MatrixReal->new_from_cols([[0, 0, 500]]);
	}

	my $upVector;
	if(defined($_->{'upVector'})){
		$upVector = Math::MatrixReal->new_from_cols([$_->{'upVector'}]);
		delete $_->{'upVector'};
	}else{
		$upVector = Math::MatrixReal->new_from_cols([[0, 1, 0]]);
	}

	my $zAxis = $lookAt - $position;
	$_->{'focus'} = $zAxis->length();
	$zAxis /= $zAxis->length();

	$upVector /= $upVector->length();
	my $xAxis = $upVector->vector_product($zAxis);
	$xAxis /= $xAxis->length();

	my $yAxis = $zAxis->vector_product($xAxis);

	# Matrix to add one dimension to the vectors
	my $tmpMatrix = Math::MatrixReal->new_from_cols([
		[1, 0, 0, 0],
		[0, 1, 0, 0],
		[0, 0, 1, 0]]);

	# Rotations matrix:
	my $matrix = Math::MatrixReal->new_from_cols([
		$tmpMatrix * $xAxis,
		$tmpMatrix * $yAxis,
		$tmpMatrix * $zAxis,
		[0, 0, 0, 1]]);

	$position = -~($tmpMatrix * $position);
	
	$matrix->assign_row(4, $position);
	$matrix->assign(4, 4, 1);

	$_->{'inverse_transform'} = $matrix->inverse();
}

sub objects{
	$_->{'objects'} = [] unless defined($_->{'objects'}) &&
		(ref($_->{'objects'}) eq 'ARRAY');
	local $_ = $_->{'objects'};

	object() for (@$_);
}

sub object{
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
	objects();

	delete $_->{'camera'}->{'inverse_transform'};
	$_;
}

1;
