var arDrone = require('ar-drone');
var client  = arDrone.createClient();

module.exports = {
    up: up,
    down: down,
    counterClockwise: counterClockwise,
    clockwise: clockwise,
    client: client,
    stop: stop_now,
    track: track
};

var width = 640;
var height = 320;
var tick = 200;
function stop_now(){
    client.stop();
}
function stop(){
    client.after(tick, function() {
        this.stop();
    });
}
function counterClockwise(){
    client.counterClockwise(.5);
    stop();
}
function down(){
    client.up(.5);
    stop();
}
function up(){
    client.up(.5);
    stop();
}
function clockwise(){
    client.clockwise(.5);
    stop();
}
function track(center){
    var x_diff = center[0]-width/2.0;
    var y_diff = center[1]-height/2.0;
    if(x_diff>50){
        clockwise();
    }
    else if(x_diff<-50){
        counterClockwise();
    }

    if(y_diff>50){
        down();
    }
    else if(y_diff<-50){
        up();
    }
}
