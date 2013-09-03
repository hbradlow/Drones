var fs = require('fs');                                                         
var PaVEParser = require('./node-dronestream/lib/PaVEParser');                             
fs.createReadStream('recorded.pave')                                                
    .pipe(new PaVEParser())                                                     
    .pipe(fs.createWriteStream('out.h264')); 
