import 'package:flutter/scheduler.dart';
import 'package:flutter/widgets.dart';

import 'led_pattern_model.dart';
import 'led_pattern_service.dart';


class LedPatternViewModel extends ChangeNotifier {
  final LedPatternService _ledPatternService;
  
  bool _busy = false;
  bool get busy => _busy;

  LedPatternViewModel(this._ledPatternService);

  LedPattern get ledPattern => _ledPatternService.ledPattern;

  void setBusy(bool value) {
    _busy = value;
    SchedulerBinding.instance.addPostFrameCallback((_)  => notifyListeners());
  }

  Future getPattern(BuildContext buildContext) async {
    setBusy(true);
    try{
      await _ledPatternService.getPattern(buildContext);
    }finally{
      setBusy(false);
    }
  }

}

class LedPatternViewModelTree extends LedPatternViewModel{
  LedPatternViewModelTree(ledPatternService): super(ledPatternService);

}

class LedPatternViewModelRing extends LedPatternViewModel{
  LedPatternViewModelRing(ledPatternService): super(ledPatternService);

}
