use strict;
use warnings;

package SharedDefs;

# load a .h file and 
sub load_shared_defs{
	my $file = shift;

	open IN, "<", $file or return "Error opening file $file";

	my %values;

	while(<IN>){
		 if(/^\s*#define\s([A-Z_0-9]+)\s(\S+).*$/){
			$values{$1} = $2;
		 }
	}

	\%values;
}

1;
