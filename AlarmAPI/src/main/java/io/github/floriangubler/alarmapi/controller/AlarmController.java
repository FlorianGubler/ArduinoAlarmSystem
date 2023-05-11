package io.github.floriangubler.alarmapi.controller;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import io.github.floriangubler.alarmapi.entity.AlarmEntity;
import io.github.floriangubler.alarmapi.repository.AlarmRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/* Controller for Alarm API and Alarm Socket */
@RestController
public class AlarmController {

    @Autowired
    private AlarmRepository repository;

    @Autowired
    private SimpMessagingTemplate template;

    // GET /api/history
    @GetMapping("/api/history")
    public List<AlarmEntity> history() {
        return repository.findAllByOrderByIdAsc();
    }

    // POST /api/alarm/{sensorId}
    @PostMapping("/api/alarm/{sensorId}")
    public void alarm(@PathVariable int sensorId) throws JsonProcessingException {
        //Create alarm entity
        AlarmEntity alarm = new AlarmEntity(sensorId);
        //Save new alarm
        repository.save(alarm);
        //Send message to socket channel
        this.template.convertAndSend("/alarm-message/alarm", alarm);
    }
}
