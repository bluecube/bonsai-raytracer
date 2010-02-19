#!/usr/bin/perl

use strict;
use warnings;

use Getopt::Long;
use List::Util qw(min max);
use POSIX qw(frexp);

use Image::Magick;

use Scene;

use constant ID_STRING => "Bonsai raytracer server v0.1";

use constant DEFAULT_CHUNKSIZE => 8000;
use constant DEFAULT_CHUNKTIMEOUT => 60 * 20;
use constant DEFAULT_RESOLUTION => 800;
use constant DEFAULT_GUI => 1;
use constant DEFAULT_EXTENSION => '.hdr';
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

print ID_STRING, "\n\n";

our $commandLine = $0 . ' ' . join(' ', @ARGV);

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

print "Will write to '$output'.\n";
outputHDR($scene);
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
		$scene->{'outputFile'} = $file;

		$scene->{'outputFile'} =~ s|\.[^./]*$||;
		$scene->{'outputFile'} .= DEFAULT_EXTENSION;
	}else{
		$scene->{'outputFile'} = $output;
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

	my @image = ();

	my $count = 0;

	my $chunkRows = max(1, int($chunkSize / $width));
	for(my $y = 0; $y < $height; $y += $chunkRows){
		my $len = min($chunkRows, $height - $y);

		push @availableChunks, {
			'top' => $y,
			'height' => $len,
			'scene' => $scene,
			};
	
		if($count & 1){
			push @image, ([0.5, 0, 0]) x ($width * $len);
		}else{
			push @image, ([0.12, 0.3, 0]) x ($width * $len);
		}
		
		++$count;
	}

	$scene->{'image'} = \@image;

	$scene->{'width'} = $width;
	$scene->{'height'} = $height;

	print "Output resolution: $width x $height; $count chunks, ",
		"$chunkRows rows/chunk ( = ", $width * $chunkRows, " px/chunk).\n";
	
	1;
}

sub outputHDR{
	my $scene = shift;

	my $data = $scene->{'image'};

	open IMAGE, '>' . $scene->{'outputFile'};

	print IMAGE "#?RADIANCE\n";
	print IMAGE $commandLine, '\n' if $commandLine;
	print IMAGE "FORMAT=32-bit_rle_rgbe\n";
	print IMAGE "SOFTWARE=", ID_STRING, "\n";
	print IMAGE "\n";
	print IMAGE "-Y ", $scene->{'height'}, " +X ", $scene->{'width'}, "\n";
	
	my $count = $scene->{'width'} * $scene->{'height'};
	for(my $i = 0; $i < $count; ++$i){
		print IMAGE float2rgbe($data->[$i]);
	}

	close IMAGE;
}

# Gets floating point RGB as parameters and returns
# 4B long string in RGBE
sub float2rgbe{
	my $tmp = shift;
	my @rgb = @$tmp;
	my $max = max(@rgb);

	my ($x, $e) = frexp($max);
	$x /= $max / 256; # $x = 256 / 2^$e;

	pack'C4', ($rgb[0] * $x, $rgb[1] * $x, $rgb[2] * $x, $e + 128);

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
