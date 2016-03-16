<html>
<head>
   <title>Data of Sensor</title>

<script type="text/javascript" src="js/highcharts.js" ></script>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>

<script type="text/javascript">
	var chart;
			$(document).ready(function() {
				var options = {
					chart: {
						renderTo: 'container',
						defaultSeriesType: 'line',
						marginRight: 130,
						marginBottom: 25
					},
					title: {
						text: 'Temperatures',
						x: -20 //center
					},
					subtitle: {
						text: '',
						x: -20
					},
					xAxis: {
						type: 'datetime',
						tickInterval: 3600 * 1000, // one hour
						tickWidth: 0,
						gridLineWidth: 1,
						labels: {
							align: 'center',
							x: -3,
							y: 20,
							formatter: function() {
								return Highcharts.dateFormat('%l%p', this.value);
							}
						}
					},
					yAxis: {
						title: {
							text: 'Visits'
						},
						plotLines: [{
							value: 0,
							width: 1,
							color: '#808080'
						}]
					},
					tooltip: {
						formatter: function() {
				                return Highcharts.dateFormat('%l%p', this.x-(1000*3600)) +'-'+ Highcharts.dateFormat('%l%p', this.x) +': <b>'+ this.y + '</b>';
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
						name: 'Count'
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
								parseInt(line[1].replace(',', ''), 10)
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


<?php
   include("conec.php");
   $link=Conection();
   $temperatures=mysql_query("select * from enviro where DATE(created) = CURDATE()  order by id desc",$link);
   $ftevents=mysql_query("select * from zones where doorname='front'  order by id desc",$link);
   $bkevents=mysql_query("select * from zones where doorname='back'  order by id desc",$link);

?>

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

<div id="container" style="width: 100%; height: 400px; margin: 0 auto"></div>
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

</table>
</body>
</html>

