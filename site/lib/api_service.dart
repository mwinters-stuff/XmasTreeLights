import 'dart:async';

import 'package:flutter/widgets.dart';
import 'package:http/http.dart' as http;
import 'http_service.dart';
import 'constants.dart';

class ApiService {
  final HttpService _httpService;

  ApiService(this._httpService);

  Future<http.Response> getTree(BuildContext context) {
    // debugPrint("UserModel.getUserDevices");
    return _httpService.get(Constants.GET_TREE, context);
  }

  Future<http.Response> getRing(BuildContext context) {
    // debugPrint("UserModel.getUserDevices");
    return _httpService.get(Constants.GET_RING, context);
  }
  Future<http.Response> setTree(BuildContext context, dynamic body) {
    // debugPrint("UserModel.controlUserDevice " + body.toString());
    return _httpService.post(Constants.SET_TREE, body, context);
  }

  Future<http.Response> setRing(BuildContext context, dynamic body) {
    // debugPrint("UserModel.controlUserDevice " + body.toString());
    return _httpService.post(Constants.SET_RING, body, context);
  }

  get(BuildContext context, String what) {
    if(what == 'ring'){
      return getRing(context);
    }
    return getTree(context);
  }
}