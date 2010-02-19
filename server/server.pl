#!/usr/bin/perl

use strict;
use warnings;

use Getopt::Long;
use List::Util qw(min max);

use Image::Magick;

use Scene;
use Gui;

use constant DEFAULT_CHUNKSIZE => 8000;
use constant DEFAULT_CHUNKTIMEOUT => 60 * 20;
use constant DEFAULT_RESOLUTION => 800;
use constant DEFAULT_GUI => 1;
use constant DEFAULT_EXTENSION => '.exr';
use constant DEFAULT_PORT => 23232;

# Configuration
our $chunkSize = DEFAULT_CHUNKSIZE;
our $widthOption;
our $heightOption;
our $timeout = DEFAULT_CHUNKTIMEOUT;
our $resume;
our $output;
our $port = DEFAULT_PORT;
our $guiEnabled = DEFAULT_GUI;

# global stuff
our @availableChunks; # Array of chunks waiting to be processed
our $scene;

print "Bonsai raytracer server.\n\n";

my $result = GetOptions(
	'help' => \&usage,
	'chunksize=i' => \$chunkSize,
	'gui!' => \$guiEnabled,
	'width=i' => \$widthOption,
	'height=i' => \$heightOption,
	'output=s' => \$output,
	'port=i' => \$port,
	'timeout=i' => \$timeout,
	'resume=s' => \$resume);

print "No input files.\n\n" and usage() unless @ARGV;
print "Only single input file is allowed.\n\n" and usage() if @ARGV > 1;

nextChunk();

print "blob:\n", $scene->{'image'}->ImageToBlob(), "\n\n";

print "Will write to '$output'.\n";
print $scene->{'image'}->Write($output);
print "Done.\n";

print "Bye bye\n";

# Get the next chunk to process
# and mark it as busy
sub nextChunk{
	scalar(@availableChunks) or loadScene() or return 0;

	my $chunk = shift @availableChunks;
	$chunk;
}

# mark the chunk as finished, put it into output array,
# update UI.
sub finishChunk{
	my $chunk = shift;
	my $data = shift;

	
}

# Load a next input file and turn it into chunks.
sub loadScene(){
	return 0 unless @ARGV;

	my $file = shift @ARGV;

	print "Loading scene from '$file'.\n";
	$scene = Scene::load($file);
	print "Loaded.\n";

	if(!defined($output)){
		$output = $file;

		$output =~ s|\.[^./]*$||;
		$output .= DEFAULT_EXTENSION;
	}

	my $width;
	my $height;

	if(defined($widthOption) && defined($heightOption)){
		if($heightOption * $scene->{'aspect'} > $widthOption){
			$width = $widthOption;
			$height = $widthOption / $scene->{'aspect'};
		}else{
			$width = $heightOption * $scene->{'aspect'};
			$height = $heightOption;
		}
	}elsif(defined($widthOption)){
		$width = $widthOption;
		$height = $widthOption / $scene->{'aspect'};
	}elsif(defined($heightOption)){
		$width = $heightOption * $scene->{'aspect'};
		$height = $heightOption;
	}else{
		if($scene->{'aspect'} > 1){
			$width = DEFAULT_RESOLUTION;
			$height = DEFAULT_RESOLUTION / $scene->{'aspect'};
		}else{
			$width = DEFAULT_RESOLUTION * $scene->{'aspect'};
			$height = DEFAULT_RESOLUTION;
		}
	}

	$width = int($width);
	$height = int($height);

	$scene->{'image'} = new Image::Magick;
	$scene->{'image'}->Set(size => $width . 'x' . $height, magick => 'RGB');
	$scene->{'image'}->ReadImage('xc:red');

	my $count = 0;

	my $chunkRows = max(1, int($chunkSize / $width));
	for(my $y = 0; $y < $height; $y += $chunkRows){
		my $len = min($chunkRows, $height - $y);

		push @availableChunks, {
			'top' => $y,
			'height' => $len,
			'scene' => $scene,
			};
	
		my $color;
		
		$scene->{'image'}->Draw(
			primitive => 'rectangle',
			fill => 'green',
			stroke => 'green',
			points => '0,' . $y . ' ' . ($width - 1) . ',' . ($y + $len - 1),
			strokewidth => 0) if($count & 1);

		++$count;
	}

	$scene->{'width'} = $width;
	$scene->{'height'} = $height;

	print "Output resolution: $width x $height; $count chunks, ",
		"$chunkRows rows/chunk ( = ", $width * $chunkRows, " px/chunk).\n";
	
	1;
}

# Print the usage info and exit
sub usage{
	my $chunkSize = DEFAULT_CHUNKSIZE;
	my $resolution = DEFAULT_RESOLUTION;
	my $gui = DEFAULT_GUI ? "enabled" : "disabled";
	my $extension = DEFAULT_EXTENSION;
	my $port = DEFAULT_PORT;
	my $timeout = DEFAULT_CHUNKTIMEOUT;

	print <<EOT;
Usage:
	$0 [OPTIONS] FILE
		Render a scene from FILE.

OPTIONS:
	--chunksize=NUMBER
		Granularity of the rendering.
		Roughly how many pixels will there be in one chunk.
		Default value is $chunkSize px.
	--gui (--nogui)
        	Enable (disable) SDL gui with preview. Defaults to $gui.
	--help
		This help.
	--output=FILE
		Save output file at this location. Defaults to INPUT_FILE with
		'$extension' substituted for the extension.
	--port=NUMBER
		Port to listen on. Default $port.
	--timeout=NUMBER
		Set the chunk processing timeout in seconds. (How much time each chunk has to be
		computed). Default value is $timeout s.
	--resume=FILE
		Filename of a file used for resuming. If this is specified, then Bonsai will try to
		resume from it and then write progress data into it.
		Resuming is disabled by default.
	--width=WIDTH
	--height=WIDTH
        	Output resolution. If only one of these is used, then the other one
		is calculated from the aspect ratio. If both parameters are specified
		then the resulting picture will be as large as possible while not exceeding
		the given width and height and keeping the original aspect ratio.
		If neither is specified then $resolution is used as both width and height.

	Option names may be abbreviated.
EOT

	exit;
}
