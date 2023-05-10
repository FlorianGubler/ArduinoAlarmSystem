var stompClient = null;

function setConnected(connected) {
    $("#connect").prop("disabled", connected);
    $("#disconnect").prop("disabled", !connected);
    if (connected) {
        $("#conversation").show();
    }
    else {
        $("#conversation").hide();
    }
    $("#alarmHistory").html("");
    loadHistory();
}

function connect() {
    var socket = new SockJS('/alarm-socket');
    stompClient = Stomp.over(socket);
    stompClient.connect({}, function (frame) {
        setConnected(true);
        stompClient.subscribe('/alarm-message/alarm', function (alarm) {
            showAlarm(JSON.parse(alarm.body));
        });
    });
}

function disconnect() {
    if (stompClient !== null) {
        stompClient.disconnect();
    }
    setConnected(false);
    console.log("Disconnected");
}

function showAlarm(alarm) {
    $("#alarmHistory").append("<tr><td>" + alarm.id + "</td><td>" + alarm.sensorId + "</td><td>" + alarm.time + "</td></tr>");
    $("#alarm-field-sensor").html(alarm.sensorId);
    $("#alarm-display").show().delay(5000).fadeOut().html();
}

function loadHistory(){
    $.get("/api/history", function(data, status){
        if(status == "success"){
            for(i in data){
                $("#alarmHistory").append("<tr><td>" + data[i].id + "</td><td>" + data[i].sensorId + "</td><td>" + data[i].time + "</td></tr>");
            }
        }
    });
}