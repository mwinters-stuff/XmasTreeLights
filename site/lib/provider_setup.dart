import 'package:flutter/widgets.dart';
import 'package:site/led_pattern_service.dart';
import 'package:site/led_pattern_view_model.dart';

import 'http_service.dart';
import 'api_service.dart';
import 'package:provider/provider.dart';


List<SingleChildCloneableWidget> providers = [
  ...independentServices,
  ...dependentServices,
  ...uiConsumableProviders
];

List<SingleChildCloneableWidget> independentServices = [
  Provider.value(value: HttpService())
];

List<SingleChildCloneableWidget> dependentServices = [
  ProxyProvider<HttpService, ApiService>(
    update: (BuildContext context, HttpService value, ApiService previous) {
      return ApiService(value);
    },
  ),
  
  ProxyProvider<ApiService, LedPatternServiceTree>(
    update: (BuildContext context, ApiService value, LedPatternServiceTree previous) {
      return LedPatternServiceTree(value);
    },
  ),

  ProxyProvider<ApiService, LedPatternServiceRing>(
    update: (BuildContext context, ApiService value, LedPatternServiceRing previous) {
      return LedPatternServiceRing(value);
    },
  ),

];

List<SingleChildCloneableWidget> uiConsumableProviders = [
  ChangeNotifierProxyProvider<LedPatternServiceTree,LedPatternViewModelTree>(
    create: (BuildContext context) {
      return null;
    }, 
    update: (BuildContext context, LedPatternServiceTree value, LedPatternViewModelTree previous) {
      return LedPatternViewModelTree(value);
    },
    
  ),
    ChangeNotifierProxyProvider<LedPatternServiceRing,LedPatternViewModelRing>(
    create: (BuildContext context) {
      return null;
    }, 
    update: (BuildContext context, LedPatternServiceRing value, LedPatternViewModelRing previous) {
      return LedPatternViewModelRing(value);
    },
    
  ),

  
  ];