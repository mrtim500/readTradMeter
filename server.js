var net = require('net');
var mysql = require('mysql');

var heleTelnetString = '';

var client = net.connect({port: 50000, host: '192.168.0.1'},
    function() { //'connect' listener
  console.log('connected to server!');
  //client.write('world!\r\n');
});
    client.setEncoding('utf8');

client.on('data', function(data) {

heleTelnetString += data;

if (data == '}'){
  console.log(heleTelnetString);
    
    var jsonString = JSON.parse(heleTelnetString);    

    var connection = mysql.createConnection({
        host: 'localhost',
        user: 'omnik',
        password: 'omnik',
        database: 'omnik',
    });

    // Try to add data to database
    connection.connect(function(error, results) {
        if (error) {
            console.log('Connection Error: ' + error.message);
            return;
        }
        console.log('Connected to MySQL');
    });

    var values = [jsonString.watt, (jsonString.Wh)];
    connection
        .query(
            'INSERT INTO kWh SET watt = ?, Wh = ?',
            values, function(error, results) {
                if (error) {
                    console.log('ClientReady Error: ' + error.message);
                    connection.end();
                    return;
                }
                console.log('Inserted: ' + results.affectedRows + ' row.');
                console.log('Id inserted: ' + results.insertId);
            });

    connection.end();    
    
    
  heleTelnetString = '';    
};    
    
});
client.on('end', function() {
  console.log('disconnected from server');
});