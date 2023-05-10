package io.github.floriangubler.alarmapi;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
@EnableAutoConfiguration
public class AlarmApiApplication {

	public static void main(String[] args) {
		SpringApplication.run(AlarmApiApplication.class, args);
	}

}
