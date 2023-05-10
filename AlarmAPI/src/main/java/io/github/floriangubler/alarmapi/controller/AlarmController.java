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

@RestController
public class AlarmController {

    @Autowired
    private AlarmRepository repository;

    @Autowired
    private SimpMessagingTemplate template;

    @GetMapping("/api/history")
    public List<AlarmEntity> history() {
        return repository.findAllByOrderByIdAsc();
    }

    @PostMapping("/api/alarm/{sensorId}")
    public void alarm(@PathVariable int sensorId) throws JsonProcessingException {
        AlarmEntity alarm = new AlarmEntity(sensorId);
        repository.save(alarm);
        this.template.convertAndSend("/alarm-message/alarm", alarm);
    }
}
