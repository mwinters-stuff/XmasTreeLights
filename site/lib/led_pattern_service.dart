import 'package:flutter/material.dart';
import 'package:http/http.dart';
import 'package:site/led_pattern_model.dart';

import 'api_service.dart';

class LedPatternService{
  final ApiService _apiService;
  final String _what;

  LedPattern _ledPattern = LedPattern();
  LedPattern get ledPattern => _ledPattern;

  LedPatternService(this._apiService, this._what);

  Future getPattern(BuildContext buildContext) async {
    final Response response  = await _apiService.get(buildContext, _what);
    _ledPattern = ledPatternFromJson(response.body);
    
  }
}

class LedPatternServiceTree extends LedPatternService{
  LedPatternServiceTree(apiService):super(apiService, "tree" );
}

class LedPatternServiceRing extends LedPatternService{
  LedPatternServiceRing(apiService):super(apiService, "tree" );
}