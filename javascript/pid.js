var arDrone = require('ar-drone');
var client  = arDrone.createClient();

module.exports = {
    up: up,
    down: down,
    counterClockwise: counterClockwise,
    clockwise: clockwise,
    client: client,
    stop: stop_now,
    track: track,
    forward: forward,
    back: back,
};

var width = 640;
var height = 320;
var tick = 200;
var safety = true;
function stop_now(){
    client.stop();
}
function stop(){
    client.after(tick, function() {
        this.stop();
    });
}
function counterClockwise(speed){
    console.log('counterClockwise');
    if(safety){
        client.counterClockwise(speed);
        client.after(tick, function() {
            this.counterClockwise(0);
        });
    }
}
function down(speed){
    console.log('down');
    if(safety){
        client.down(speed);
        client.after(tick, function() {
            this.up(0);
        });
    }
}
function up(speed){
    console.log('up');
    if(safety){
        client.up(speed);
        client.after(tick, function() {
            this.up(0);
        });
    }
}
function back(speed){
    console.log('back');
    if(safety){
        client.back(speed);
        client.after(tick, function() {
            this.back(0);
        });
    }
}
function forward(speed){
    console.log('forward');
    if(safety){
        client.front(speed);
        client.after(tick, function() {
            this.front(0);
        });
    }
}
function clockwise(speed){
    console.log('clockwise');
    if(safety){
        client.clockwise(speed);
        client.after(tick, function() {
            this.clockwise(0);
        });
    }
}
function track(center){
    var x_diff = center[0]-width/2.0;
    var x_speed = Math.abs(x_diff/width);
    var y_diff = center[1]-height/2.0;
    var y_speed = Math.abs(y_diff/height);
    if(x_diff>0){
        clockwise(x_speed);
    }
    else if(x_diff<-0){
        counterClockwise(x_speed);
    }

    if(y_diff>0){
        down(y_speed);
    }
    else if(y_diff<-0){
        up(y_speed);
    }
}
