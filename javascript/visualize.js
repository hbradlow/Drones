var http = require("http");

var server = http.createServer(function(req, res) {
  require("fs").createReadStream(__dirname + "/dronestream.html").pipe(res);
});

require("dronestream").listen(server); 
server.listen(5555);
