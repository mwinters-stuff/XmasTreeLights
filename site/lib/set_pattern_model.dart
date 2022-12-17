// To parse this JSON data, do
//
//     final setPattern = setPatternFromJson(jsonString);

import 'dart:convert';

SetPattern setPatternFromJson(String str) => SetPattern.fromJson(json.decode(str));

String setPatternToJson(SetPattern data) => json.encode(data.toJson());

class SetPattern {
    int currentIndex;
    bool random;
    int overrideIndex;
    List<String> sequences;

    SetPattern({
        this.currentIndex,
        this.random,
        this.overrideIndex,
        this.sequences,
    });

    factory SetPattern.fromJson(Map<String, dynamic> json) => SetPattern(
        currentIndex: json["current_index"],
        random: json["random"],
        overrideIndex: json["override_index"],
        sequences: List<String>.from(json["sequences"].map((x) => x)),
    );

    Map<String, dynamic> toJson() => {
        "current_index": currentIndex,
        "random": random,
        "override_index": overrideIndex,
        "sequences": List<dynamic>.from(sequences.map((x) => x)),
    };
}
