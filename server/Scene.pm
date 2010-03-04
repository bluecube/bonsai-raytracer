use strict;
use warnings;

package Scene;

use Data::Dumper;
use JSON::XS;
use Math::VectorReal;
use Math::MatrixReal;

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

# number of samples per unit of aperture area.
use constant DEFAULT_APERTURE_SAMPLES => 1024;

# process camera settings and normalize its dimensions.
# The real sensor is always 1 unit wide and (1 / aspect) high.
sub cameraSettings{
	local $_ = shift;
	$_->{'camera'} = {} unless defined($_->{'camera'});
	$_ = $_->{'camera'};

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
	if(defined($_->{'apertureSamples'})){
		$samples = $samples * $_->{'apertureSamples'};
	}else{
		$samples = $samples * DEFAULT_APERTURE_SAMPLES;
	}
	$samples = int($samples) + 1;
	$_->{'apertureSamples'} = $samples;

	# transformation matrix
	my $position = vector(@{$_->{'position'}});
	my $lookAt = vector(@{$_->{'lookAt'}});
	my $up = vector(@{$_->{'upVector'}});

	my $zAxis = $lookAt - $position;
	$zAxis->norm();

	$up->norm();
	my $xAxis = $upVector x $zAxis;
	$xAxis->norm();

	my $yAxis = $zAxis x $xAxis;

}

sub load{
	my $filename = shift;
	
	local $/;
	undef $/;

	open(my $fh, '<', $filename) or die $!;

	my $file = decode_json <$fh>;

	close $fh;

	print Dumper($file);

	# First process the camera settings
	cameraSettings($file);

	print Dumper($file);
exit;

	$file;
}

1;
