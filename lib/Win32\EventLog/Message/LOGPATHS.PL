use Win32::EventLog::Message;

$LogName = lc shift @ARGV || die "Syntax:\n$0 <Event Log Name> [source [source2 [...] ] ]\n";
print "\u$LogName Event Log:\n";

$~ = MessageLogHeader;
write;

$~ = MessageLogPath;

if( scalar @ARGV )
{
    push( @List, @ARGV );
}
else
{
    GetSourceList( $LogName, \@List );
}

$Num = 0;
foreach $Name ( sort( @List ) )
{
    if( GetSource( $LogName, $Name, \%Config ) )
    {
        $Num++;
        $Path = join( " " x 30, split( /[,;]/, $Config{path} ) );
        write;
    }
}

format MessageLogHeader =
  @>>> @<<<<<<<<<<<<<<<<<<<<<<<<<<< @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  "", "Source Log Name",            "Path To Message Table"
  ---- ---------------------------- ------------------------------------------
.

format MessageLogPath =
  @>>> @<<<<<<<<<<<<<<<<<<<<<<<<<<< ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  $Num, $Name,                      $Path
~                                   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    $Path
~                                   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    $Path
~                                   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    $Path
~                                   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    $Path
~                                   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    $Path
.