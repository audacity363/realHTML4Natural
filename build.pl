use Getopt::Long;
use File::Basename;
use File::Copy;

sub printUsage {
    print "Usage: $0\n";
    print "\t--cc           Path to your comiler gcc or xlc\n";
    print "\t--ar           Path to your ar binary\n";
    print "\t--javac        Path to your java compiler\n";
    print "\t--javah        Path to your javah utility\n";
    print "\t--jar          Path to your jar utility\n";
    print "\t--tomcatpath   Path to the root folder of your tomcat installation\n";
    print "\t--jnipath      Path to the folder with the jni.h header file\n";
    exit(0);
}

sub searchBin {
    my $bin_name = @_[0];
    if(length $bin_name == 0) { return ""; }
    my $tool_path = '';

    for my $path ( split /:/, $ENV{PATH} ) {
        if ( -f "$path/$bin_name" && -x _ ) {
            $tool_path = "$path/$bin_name";
            last;
        }
    }
    return $tool_path;
}

my $cc = "";
my $cc_lf1_so = "";
my $cc_lf2_so = "";
my $cc_cargs1 = "";
my $cc_cargs2 = "";
my $cc_cargs_so = "";
my $ar = "";
my $javac = "";
my $javah = "";
my $tomcatpath = "";
my $jnipath = "";
my $jni = "";
my $help = 0;

GetOptions("cc=s" => \$cc, 
           "ar=s" => \$ar, 
           "javac=s" => \$javac,
           "javah=s" => \$javah,
           "jni=s" => \$jni,
           "help!" => \$help,
           "usage!" => \$help);

if($help) { printUsage(); }

if(!$cc) {
    $cc = searchBin("gcc");
    if(!$cc) {
        $cc = searchBin("xlc");
    }
}
die "Could not find gcc or xlc. Please make sure a C compiler is installed" unless($cc);

if($cc =~ /.*gcc/ != 0) {
    print "Using gcc\n";
    $cc_lf1_so = "-shared -Wl,--no-undefined";
    $cc_lf2_so = "";
    $cc_cargs1 = "-g -c -fPIC";
    $cc_cargs2 = "";
    $cc_cargs_so = "-c -g -fPIC";
} else {
    print "Using xlc\n";
    $cc_lf1_so = "-G -bsymbolic -bexpall -bnoentry";
    $cc_lf2_so = "-b64 -ldl -blibpath:/usr/lib/threads:/usr/lib:/lib -lc_r";
    $cc_cargs1 = "-g -c";
    $cc_cargs2 = "";
    $cc_cargs_so = "-c -g -q64 -fpic";
}

if(!$ar) { $ar = searchBin("ar"); }
die "Could not find ar. Please make sure ar is installed" unless($ar);

if(!$javac) { $javac = searchBin("javac"); }
die "Could not find javac. Please make sure a java SDK is installed" unless($javac);

if(!$javah) { $javah = searchBin("javah"); }
die "Could not find javah. Please make sure a java SDK is installed" unless($javac);

if(!$jar) { $jar= searchBin("jar"); }
die "Could not find jar. Please make sure a java SDK is installed" unless($javac);

if(!$tomcatpath) {
    print "I will now search for your tomcat installation. This is the time to grab a coffee. It could take a while...\n";
    my $findret = `find / -name "servlet-api.jar" 2>/dev/null`;
    die "Could not find any servlet-api.jar. Are you sure that a tomcat is installed?" unless($findret);

    my @tomcatlocations = split /\n/, $findret;
    if(scalar @tomcatlocations > 1) {
        my $i = 0;
        foreach(@tomcatlocations) {
            print "[$i] $_\n";
            $i += 1;
        }
        print "Found multiple entries. Please select the right one\n> ";
        my $selection = <STDIN>;
        chomp $selection;
        if($selection > ((scalar @tomcatlocations)-1)) { 
            die "$selection was not listed.... Use 0 - ".($selection-1)."!";
        }
        $tomcatpath = @tomcatlocations[$selection];
    } else {
        chomp $findret;
        $tomcatpath = $findret;
    }
	my $tomcatlibpath = dirname($tomcatpath);
	$tomcatpath .= ":" . $tomcatlibpath . "/jsp-api.jar";
}

if(!$jnipath) {
    print "I will now search for your jni header files. When your coffee is empty grab another one\n";
    $findret = `find / -name "jni.h" 2>/dev/null`;
    die "Could not find any jni.h. Are you sure that a Java SDK is installed?" unless($findret);

    my @jnilocations = split /\n/, $findret;
    if(scalar @jnilocations > 1) {
        $i = 0;
        my $path;
        foreach(@jnilocations) {
            $path = dirname($_);
            print "[$i] $path\n";
            $i += 1;
        }
        print "Found multiple entries. Please select the right one\n> ";
        my $selection = <STDIN>;
        chomp $selection;
        if($selection > ((scalar @jnilocations)-1)) { 
            die "$selection was not listed.... Use 0 - ".($selection-1)."!";
        }
        $jnipath = dirname(@jnilocations[$selection])."/";
    } else {
        chomp $findret;
        $jnipath = dirname($findret)."/";
    }
}



print "Summary:\n";
print "\tCC:           $cc\n";
print "\tar:           $ar\n";
print "\tjavac:        $javac\n";
print "\tjavah:        $javah\n";
print "\tservlet-api:  $tomcatpath\n";
print "\tJNI location: $jnipath\n";
print "Are these information correct? (Y/n)\n> ";
my $yesno = <STDIN>;
chomp $yesno;

if($yesno eq "n" || $yesno eq "N") {
    print "So the script could not find the right locations? Than please fix the\n";
    print "script or specify the locations manualy. See usage\n";
    exit(1);
}

print "Building Makefile\n";
copy("Makefile.rh4n","Makefile") or die "Copy failed: $!";

$jnipath = "-I".$jnipath." -I".$jnipath."/linux/";

`perl -pi -e "s~{{CC}}~$cc~g" ./Makefile`;
`perl -pi -e "s~{{AR}}~$ar~g" ./Makefile`;
`perl -pi -e "s~{{JAR}}~$jar~g" ./Makefile`;
`perl -pi -e "s~{{JAVAC}}~$javac~g" ./Makefile`;
`perl -pi -e "s~{{JAVAH}}~$javah~g" ./Makefile`;
`perl -pi -e "s~{{SERVLETPATH}}~$tomcatpath~g" ./Makefile`;
`perl -pi -e "s~{{JNIPATH}}~$jnipath~g" ./Makefile`;
`perl -pi -e "s~{{CCLF1SO}}~$cc_lf1_so~g" ./Makefile`;
`perl -pi -e "s~{{CCLF2SO}}~$cc_lf2_so~g" ./Makefile`;
`perl -pi -e "s~{{CCCARGS1}}~$cc_cargs1~g" ./Makefile`;
`perl -pi -e "s~{{CCCARGS2}}~$cc_cargs2~g" ./Makefile`;
`perl -pi -e "s~{{CCCARGSSO}}~$cc_cargs_so~g" ./Makefile`;

