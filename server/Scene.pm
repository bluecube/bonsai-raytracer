use strict;
use warnings;

package Scene;

use Data::Dumper;
use JSON::XS;

sub load{
	my $filename = shift;
	
	local $/;
	undef $/;

	open(my $fh, '<', $filename) or die $!;

	my $file = decode_json <$fh>;

	close $fh;

	print Dumper($file);
exit;


	{aspect => 16/9};
}

1;
