fs = require('fs');
http = require('http');
url = require('url');

var mediaSource = new MediaSource();
var sourceBuffer = mediaSource.addSourceBuffer('video/webm; codecs="vorbis,vp8"');
sourceBuffer.append(new Uint8Array(data));

http.createServer(function(req, res){
  var request = url.parse(req.url, true);
  var action = request.pathname;

  if (action == '/logo.png') {
     var img = fs.readFileSync('./logo.png');
     res.writeHead(200, {'Content-Type': 'image/png' });
     res.end(img, 'binary');
  } else { 
     res.writeHead(200, {'Content-Type': 'text/plain' });
     res.end('Hello World \n');
  }
}).listen(8080, '127.0.0.1');
