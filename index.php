<html>
<head>
   <title>Data of Sensor</title>

<script type="text/javascript" src="../highcharts/js/highcharts.js" ></script>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script>


<?php
//   include("conec.php");
//   $link=Conection();
//   $temperatures=mysql_query("select * from enviro where DATE(created) = CURDATE()  order by id desc",$link);
//   $ftevents=mysql_query("select * from zones where doorname='front'  order by id desc",$link);
//   $bkevents=mysql_query("select * from zones where doorname='back'  order by id desc",$link);

?>

<script type="text/javascript">
 var now =Date.now();
 var twentyfour = now - (28 * 60 * 60 * 1000);
 var fortyeight = now - (52 * 60 * 60 * 1000);
    var chart;
            $(document).ready(function() {
                var options = {
		chart: {
                        renderTo: 'container',
                        type: 'line'
                           },
                    title: {
                        text: 'Basement Temperature',
                       // x: -20 //center
                    },
                    subtitle: {
                        text: '',
                        x: -20
                    },
                    xAxis: {
           		 plotBands: [{
                		color: '#EEEEEE',
                		from: fortyeight,
               			to: twentyfour
           		 }],
                        type: 'datetime',
                        tickInterval: 3600 * 1000 *3, // one hour
                        tickWidth: 0,
                        gridLineWidth: 1,
                        labels: {
                            align: 'center',
                            x: -3,
                            y: 20,
                            formatter: function() {
                                return Highcharts.dateFormat('%a %l%p', this.value);
                            }

                        }
                    },
                    yAxis: {
                        title: {
                            text: 'Temperature (C)'
                        },
                        plotLines: [{
                            value: 0,
                            width: 1,
                            color: '#808080'
                        }]
                    },
                    tooltip: {
			formatter: function() {
			return Highcharts.dateFormat('%l:%M%p', this.x) +': <b>'+ this.y + 'C</b>';
			}
                     },
                    legend: {
                        layout: 'vertical',
                        align: 'right',
                        verticalAlign: 'top',
                        x: -10,
                        y: 100,
                        borderWidth: 0
                    },
                    series: [{
                        name: 'Basement'
                    }]
                }
                // Load data asynchronously using jQuery. On success, add the data
                // to the options and initiate the chart.
                // This data is obtained by exporting a GA custom report to TSV.
                // http://api.jquery.com/jQuery.get/
                jQuery.get('data.php', null, function(tsv) {
                    var lines = [];
                    traffic = [];
                    try {
                        // split the data return into lines and parse them
                        tsv = tsv.split(/\n/g);
                        jQuery.each(tsv, function(i, line) {
                            line = line.split(/\t/);
                            date = Date.parse(line[0] +' UTC');
                            traffic.push([
                                date,
                                parseFloat(line[1].replace(',', ''), 10)
                            ]);
                        });
                    } catch (e) {  }
                    options.series[0].data = traffic;
                    chart = new Highcharts.Chart(options);
                });
            });
</script>

</head>

<body>

<h1>Data from the Sensor</h1>
<a href="zones.html">Zones</a>

<div id="container" style="width: 100%;  margin: 0 auto"></div>

<!--Removing this because of charting
<h2>Today's Temp and Humidity</h2>
<table border="1" cellspacing="1" cellpadding="1">
      <tr>
         <td>&nbsp;Name &nbsp;</td>
         <td>&nbsp;Temperature &nbsp;</td>
         <td>&nbsp;Humidity &nbsp;</td>
         <td>&nbsp;Date &nbsp;</td>
       </tr>
<?php
   while($row = mysql_fetch_array($temperatures)) {
printf("<tr><td> &nbsp;%s </td><td>%s</td><td>%s</td><td> &nbsp;%s&nbsp; </td></tr>",$row["name"], $row["temp"], $row["humi"], $row["created"]);
   }
   mysql_free_result($result);
 ?>
</table>

<h2>Front Door</h2>
<table border="1" cellspacing="1" cellpadding="1">
      <tr>
         <td>&nbsp;Door &nbsp;</td>
         <td>&nbsp;Event &nbsp;</td>
         <td>&nbsp;Time &nbsp;</td>
         <td>&nbsp;Date &nbsp;</td>
       </tr>
<?php
   while($row = mysql_fetch_array($ftevents)) {
printf("<tr><td> &nbsp;%s </td><td>%s</td><td>%s</td><td> &nbsp;%s&nbsp; </td></tr>",$row["doorname"], $row["event"], $row["deltatime"], $row["created"]);
   }
   mysql_free_result($result);
 ?>

</table>

<h2>Back Door</h2>
<table border="1" cellspacing="1" cellpadding="1">
      <tr>
         <td>&nbsp;Door &nbsp;</td>
         <td>&nbsp;Event &nbsp;</td>
         <td>&nbsp;Time &nbsp;</td>
         <td>&nbsp;Date &nbsp;</td>
       </tr>
<?php
   while($row = mysql_fetch_array($bkevents)) {
printf("<tr><td> &nbsp;%s </td><td>%s</td><td>%s</td><td> &nbsp;%s&nbsp; </td></tr>",$row["doorname"], $row["event"], $row["deltatime"], $row["created"]);
   }
   mysql_free_result($result);
 ?>
</table>
--->
</table>
</body>
</html>

