package io.github.floriangubler.alarmapi.repository;

import io.github.floriangubler.alarmapi.entity.AlarmEntity;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface AlarmRepository extends JpaRepository<AlarmEntity, Long> {
    public List<AlarmEntity> findAllByOrderByIdAsc();
}
