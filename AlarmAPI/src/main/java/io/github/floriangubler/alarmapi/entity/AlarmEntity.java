package io.github.floriangubler.alarmapi.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.Id;

import java.time.OffsetDateTime;

/* Entity for an Alarm Trigger in Alarm History */
@Entity
public class AlarmEntity {

    @Id
    @GeneratedValue
    private Long id;

    private OffsetDateTime time;

    private int sensorId;

    public AlarmEntity(){ }

    public AlarmEntity(int sensorId) {
        this.time = OffsetDateTime.now();
        this.sensorId = sensorId;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public OffsetDateTime getTime() {
        return time;
    }

    public void setTime(OffsetDateTime time) {
        this.time = time;
    }

    public int getSensorId() {
        return sensorId;
    }

    public void setSensorId(int alarmId) {
        this.sensorId = alarmId;
    }
}
