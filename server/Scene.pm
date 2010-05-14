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

# number of samples per (1 / PI) of aperture area.
# Computation time is O(n) to this value.
use constant DEFAULT_APERTURE_QUALITY => 1e3;

# object types and their handlers:
my %objectTypes = (
	polygonal => sub{
		my $ret = Raw::load($_->{'file'});

		die $ret unless ref($ret);

		delete $_->{'file'};
		$_->{'vertices'} = $ret->{'vertices'};
		$_->{'polygons'} = $ret->{'polygons'};
	},

	CSG => sub{
		die "Operation must be specified" unless defined $_->{'operation'};
		objects();
	}
);

sub readMatrix{
	my $t = shift;

	die "4x4 matrix (array of 16 numbers) expected" unless
		ref($t) eq 'ARRAY' && @$t == 16;

	$t = Math::MatrixReal->new_from_rows([
			[@{$t}[0 .. 3]],
			[@{$t}[4 .. 7]],
			[@{$t}[8 .. 11]],
			[@{$t}[12 .. 15]]]);
	}

	$t;
}

# process camera settings and normalize its dimensions.
# The real sensor is always 1 unit wide and (1 / aspect) high.
sub cameraSettings{
	$_->{'camera'} = {} unless defined($_->{'camera'});
	local $_ = $_->{'camera'};

	# focal length
	$_->{'focalLength'} = DEFAULT_FOCAL_LENGTH unless defined($_->{'focalLength'});

	# sensor size
	if(!defined($_->{'sensorSize'})){
		$_->{'aspect'} = DEFAULT_ASPECT;
		$_->{'focalLength'} /= DEFAULT_SENSOR_WIDTH;
	}elsif(ref($_->{'sensorSize'}) eq 'SCALAR'){
		$_->{'aspect'} = DEFAULT_ASPECT;
		$_->{'focalLength'} /= $$_->{'sensorSize'} + 0;
	}elsif(ref($_->{'sensorSize'}) eq 'ARRAY'){
		$_->{'aspect'} = $_->{'sensorSize'}->[0] / $_->{'sensorSize'}->[1];
		$_->{'focalLength'} /= $_->{'sensorSize'}->[0];
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

	my $samples = $_->{'apertureDiameter'} * $_->{'apertureDiameter'};
	if(defined($_->{'apertureQuality'})){
		$samples = $samples * $_->{'apertureQuality'};
	}else{
		$samples = $samples * DEFAULT_APERTURE_QUALITY;
	}
	delete $_->{'apertureQuality'};
	$samples = int($samples) + 1;
	$_->{'apertureSamples'} = $samples;

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

	die("Object type must be specified") unless defined($type);


	&{$objectTypes{$type}}() if defined($objectTypes{$type});
	#print "Object: $type\n";

	

	if(defined($_->{'transform'})){
		$_->{'transform'} = readMatrix($_->{'transform'})
	}else{
		$_->{'transform'} = 
	}
	
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

#	print Dumper($_);

exit;
	$_;
}

1;
