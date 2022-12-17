// To parse this JSON data, do
//
//     final ledPattern = ledPatternFromJson(jsonString);

import 'dart:convert';

LedPattern ledPatternFromJson(String str) => LedPattern.fromJson(json.decode(str));

String ledPatternToJson(LedPattern data) => json.encode(data.toJson());

class LedPattern {
    int heap;
    String ssid;
    String apAddress;
    int pin;
    int leds;
    int current;
    int currentIndex;
    String currentName;
    bool random;
    int overrideIndex;
    List<String> sequences;

    LedPattern({
        this.heap,
        this.ssid,
        this.apAddress,
        this.pin,
        this.leds,
        this.current,
        this.currentIndex,
        this.currentName,
        this.random,
        this.overrideIndex,
        this.sequences,
    });

    factory LedPattern.fromJson(Map<String, dynamic> json) => LedPattern(
        heap: json["heap"],
        ssid: json["ssid"],
        apAddress: json["ap_address"],
        pin: json["pin"],
        leds: json["leds"],
        current: json["current"],
        currentIndex: json["current_index"],
        currentName: json["current_name"],
        random: json["random"],
        overrideIndex: json["override_index"],
        sequences: List<String>.from(json["sequences"].map((x) => x)),
    );

    Map<String, dynamic> toJson() => {
        "heap": heap,
        "ssid": ssid,
        "ap_address": apAddress,
        "pin": pin,
        "leds": leds,
        "current": current,
        "current_index": currentIndex,
        "current_name": currentName,
        "random": random,
        "override_index": overrideIndex,
        "sequences": List<dynamic>.from(sequences.map((x) => x)),
    };
}
