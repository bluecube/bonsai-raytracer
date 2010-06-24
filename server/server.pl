#!/usr/bin/perl

use strict;
use warnings;

use threads;
use threads::shared;
use Getopt::Long;
use List::Util qw(min max);
use Socket;
use Term::ProgressBar;
use Data::Dumper;
use JSON::XS;

use Scene;
use RadianceHDR;
use SharedDefs;

use constant ID_STRING => "Bonsai raytracer server v0.1";

use constant DEFAULT_CHUNKSIZE => 8000;
use constant DEFAULT_CHUNKTIMEOUT => 60 * 20;
use constant DEFAULT_RESOLUTION => 800;
use constant DEFAULT_EXTENSION => '.hdr';
use constant DEFAULT_PORT => 23232;

our $sharedDefs = SharedDefs::load_shared_defs('../client/shared_defs.h');

# Configuration

our $chunkSize = DEFAULT_CHUNKSIZE;
our $widthOption;
our $heightOption;
our $timeout = DEFAULT_CHUNKTIMEOUT;
our $resume;
our $output;
our $port = $sharedDefs->{DEFAULT_PORT};

# Status stuff
our $status;

sub initStatus{
	my $scene = shift;
	$status = new Term::ProgressBar({
		name => 'Rendering',
		count => $scene->{'height'},
		remove => 1,
		ETA => 'linear',});
}

sub updateStatusMessage{
	my $scene = shift;

	my $totalChunks = $scene->{'chunkCount'};
	my $waitingChunks = scalar @{$scene->{'availableChunks'}};
	my $finishedChunks = $scene->{'finishedChunkCount'};
	my $busyChunks = $totalChunks - $finishedChunks - $waitingChunks;

	$status->message("Done / busy / total chunks: $finishedChunks / $busyChunks / $totalChunks");
}

sub updateStatus{
	my $scene = shift;
	
	$status->update($scene->{'finishedRows'});
	updateStatusMessage($scene);
}

sub statusPrint{
	$status->message(join ' ', @_);
}

# Get the next chunk to process
# and mark it as busy
sub nextChunk{
	my $scene = shift;
	scalar(@{$scene->{'availableChunks'}}) or loadScene() or return 0;

	my $chunk = shift @{$scene->{'availableChunks'}};

	updateStatusMessage($scene);
	$chunk;
}

sub failChunk{
	my $chunk = shift;
	unshift @{$chunk->{'scene'}->{'availableChunks'}}, $chunk;

	updateStatus($chunk->{'scene'});
}

# mark the chunk as finished, put it into output array,
# update UI.
sub finishChunk{
	my $chunk = shift;
	my $data = shift;
	my $scene = $chunk->{'scene'};

	my $image = $scene->{'image'};

	my $offset = $chunk->{'top'} * $scene->{'width'};
	my $length = $chunk->{'height'} * $scene->{'width'};

	((scalar @$data) == $length) or die "Wrong size of reply.";

	splice(@{$scene->{'image'}}, $offset, $length, @{$data});

	$scene->{'finishedChunkCount'} ++;
	$scene->{'finishedRows'} += $chunk->{'height'};
	updateStatus($scene);
}

# Load a next input file and turn it into chunks.
sub loadScene(){
	return 0 unless @ARGV;

	my $file = shift @ARGV;

	print "Loading scene from '$file'.\n";
	my $scene = {};

	my $data = Scene::load($file);
	print "Loaded.\n";

	if(!defined($output)){
		$scene->{'outputFile'} = $file;

		$scene->{'outputFile'} =~ s|\.[^./]*$||;
		$scene->{'outputFile'} .= DEFAULT_EXTENSION;
	}else{
		$scene->{'outputFile'} = $output;
	}

	my $title;
	if(defined($data->{'title'})){
		$title = $data->{'title'};
	}else{
		$title = $file;
		$title =~ s|\.[^./]*$||;
		$title =~ s|^.*/||;
		$data->{'title'} = $title;
	}

	print "Scene title: \"$title\".\n";

	my $width;
	my $height;

	my $aspect = $data->{'camera'}->{'aspect'};

	if(defined($widthOption) && defined($heightOption)){
		if($heightOption * $aspect > $widthOption){
			$width = $widthOption;
			$height = $widthOption / $aspect;
		}else{
			$width = $heightOption * $aspect;
			$height = $heightOption;
		}
	}elsif(defined($widthOption)){
		$width = $widthOption;
		$height = $widthOption / $aspect;
	}elsif(defined($heightOption)){
		$width = $heightOption * $aspect;
		$height = $heightOption;
	}else{
		if($aspect > 1){
			$width = DEFAULT_RESOLUTION;
			$height = DEFAULT_RESOLUTION / $aspect;
		}else{
			$width = DEFAULT_RESOLUTION * $aspect;
			$height = DEFAULT_RESOLUTION;
		}
	}

	$width = int($width);
	$height = int($height);

	my $count = 0;

	my $chunkRows = max(1, int($chunkSize / $width));
	for(my $y = 0; $y < $height; $y += $chunkRows){
		my $len = min($chunkRows, $height - $y);

		push @{$scene->{'availableChunks'}}, {
			'top' => $y,
			'height' => $len,
			'scene' => $scene,
			'data' => encode_json({'top' => $y, 'height' => $len, 'type' => 'chunk'}),
			};
	
		++$count;
	}

	my @image = ([0, 0, 0]) x ($width * $height);
	print(scalar(@image));
	$scene->{'image'} = \@image;

	$scene->{'width'} = $width;
	$scene->{'height'} = $height;

	# For the status display
	$scene->{'chunkCount'} = $count;
	$scene->{'finishedChunkCount'} = 0;
	$scene->{'finishedRows'} = 0;

	print "Output resolution: $width x $height; $count chunks, ",
		"$chunkRows rows/chunk ( = ", $width * $chunkRows, " px/chunk).\n";
	
	$data->{'width'} = $width + 0;
	$data->{'height'} = $height + 0;
	$data->{'type'} = 'scene';
	$scene->{'data'} = encode_json($data);

	$scene;
	#shared_clone($scene);
}

sub check_hello{
	my $message = shift;

	ref($message) eq 'ARRAY' &&
	$message->[0] eq 'Hello' &&
	ref($message->[2]) eq 'ARRAY' &&
	scalar(@{$message->[2]}) == 3;

}

sub worker{
	local *CLIENT = shift;
	my $scene = shift;
	my $port = shift;
	my $iaddr = shift;

	binmode(CLIENT, ":unix");

	statusPrint "Connection from ", inet_ntoa($iaddr), " at port $port";

	my $message = decode_json(<CLIENT>);
	
	if(!check_hello($message)){
		statusPrint "Protocol error (hello)";
		return;
	}

	my $name = $message->[1];
	my @version = @{$message->[2]};
	statusPrint "Client \"$name\" is version ", join(".", @version);

	if($version[0] != $sharedDefs->{'CLIENT_VERSION_MAJOR'} ||
		$version[1] != $sharedDefs->{'CLIENT_VERSION_MINOR'}){
		statusPrint "Wrong version";
		close CLIENT;
		return;
	}
	my $sceneSent = 0;
	while(1){
		my $chunk = nextChunk($scene);

		last unless $chunk;
		
		if(!$sceneSent){
			$sceneSent = 1;
			print CLIENT ($scene->{'data'} . "\n");
		}
		print CLIENT ($chunk->{'data'} ."\n");

		my $reply = decode_json(<CLIENT>);

		finishChunk($chunk, $reply);
		
		#failChunk($chunk);
	}

	print CLIENT encode_json({type => 'finished'}), "\n";

	close CLIENT;

};

# Print the usage info and exit
sub usage{
	my $chunkSize = DEFAULT_CHUNKSIZE;
	my $resolution = DEFAULT_RESOLUTION;
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

print ID_STRING, "\n\n";

our $commandLine = $0 . ' ' . join(' ', @ARGV);

my $result = GetOptions(
	'help' => \&usage,
	'chunksize=i' => \$chunkSize,
	'width=i' => \$widthOption,
	'height=i' => \$heightOption,
	'output=s' => \$output,
	'port=i' => \$port,
	'timeout=i' => \$timeout);

print "No input files.\n\n" and usage() unless @ARGV;
print "Only single input file is allowed.\n\n" and usage() if @ARGV > 1;

my $scene = loadScene();

initStatus($scene);

#the server code is shamelessly copied from example in perlipc.
statusPrint "Starting the server\n";
socket SERVER, PF_INET, SOCK_STREAM, getprotobyname('tcp') or die "socket: $!";
setsockopt SERVER, SOL_SOCKET, SO_REUSEADDR, 1 or die "setsockopt: $!";
bind SERVER, sockaddr_in($port, INADDR_ANY) or die "bind: $!";
listen SERVER, SOMAXCONN or die "listen: $!";
statusPrint "Server started, listening on port $port.\n";

#while(1){
	my $paddr = accept(CLIENT, SERVER) || do {
			# try again if accept() returned because a signal was received
			next if $!{EINTR};
			die "accept: $!";
		};
	my ($port, $iaddr) = sockaddr_in($paddr);

	worker(*CLIENT, $scene, $port, $iaddr);
#	my $thr = threads->create('worker', *CLIENT, $scene, $port, $iaddr);
#	$thr->detach();
#}

print "Writing to '", $scene->{'outputFile'} ,"'.\n";
RadianceHDR::outputHDR(
	$scene->{'outputFile'},
	$scene->{'width'},
	$scene->{'height'},
	$scene->{'image'},
	$commandLine,
	ID_STRING);

print "Bye bye\n";

