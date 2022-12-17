import 'dart:async';

import 'package:flutter/widgets.dart';
import 'package:http/http.dart' as http;


class HttpService {
  final client = new http.Client();

  Future<http.Response> get(String url, BuildContext context) async {
    try {
      return await client.get(url);
    } catch (e) {
      rethrow;
    }
  }

  Future<http.Response> post(String url, body, BuildContext context) async {
    try {
      return await client.post(url, body: body);
    } catch (e) {
      rethrow;
    }
  }

}