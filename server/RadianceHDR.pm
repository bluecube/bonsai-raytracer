use strict;
use warnings;

use POSIX qw(frexp);
use List::Util qw(min max);

package RadianceHDR;

#write radiance hdr to a given file.
sub outputHDR{
	my $filename = shift;
	my $width = shift;
	my $height = shift;
	my $data = shift;
	my $commandline = shift;
	my $idstring = shift;

	open IMAGE, ">$filename";

	print IMAGE "#?RADIANCE\n";
	print IMAGE $commandline, '\n' if $commandline;
	print IMAGE "FORMAT=32-bit_rle_rgbe\n";
	print IMAGE "SOFTWARE=$idstring\n" if $idstring;
	print IMAGE "\n";
	print IMAGE "-Y ", $height, " +X ", $width, "\n";
	
	print IMAGE float2rgbe($_) for(@$data);

	close IMAGE;
}

# Gets floating point RGB as parameters and returns
# 4B long string in RGBE
sub float2rgbe{
	my $tmp = shift;
	my @rgb = @$tmp;
	my $max = List::Util::max(@rgb);

	my ($x, $e) = POSIX::frexp($max);
	$x /= $max / 256; # $x = 256 / 2^$e;

	pack'C4', ($rgb[0] * $x, $rgb[1] * $x, $rgb[2] * $x, $e + 128);

}

1;
