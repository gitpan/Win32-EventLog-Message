use Win32::EventLog::Message;


$Machine = "" unless( $Machine = shift @ARGV );

$Num = 0;
foreach $Name ( sort( @List ) )
{
    if( GetEventLogList( $Machine, \@Logs ) )
    {
        $Num++;
        foreach my $LogName ( sort( @Logs ) )
        {
            print "$LogName\n";
        }
    }
}
