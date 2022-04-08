import 'dart:async';
import 'dart:math';
import 'dart:typed_data';

import 'package:betrayal/src/plugin.dart';
import 'package:betrayal/src/image.dart';
import 'package:betrayal/src/win_icon.dart';

import 'package:flutter/widgets.dart';
import 'package:flutter/foundation.dart';

part 'widgets.dart';

class TrayIcon {
  static final BetrayalPlugin _plugin = BetrayalPlugin();
  static final Map<Id, TrayIcon> _allIcons = {};
  static final Random _random = Random();

  /// This [Id] is used by the operating system
  /// to identify the icon and address it
  /// when it has been clicked.
  static Id _newId() => _random.nextInt(0x8FFF - 0x7FFF);

  /// The id used by Windows to distinguish this icon.
  final Id _id;
  bool _isVisible = false;
  bool get isVisible => _isVisible;

  /// Has this class be disposed?
  bool _isActive = false;
  bool get isActive => _isActive;

  /// This [StackTrace] is created when [dispose] is called.
  /// That way it's possible to debug
  /// when a [TrayIcon] has been disposed of too early.
  late StackTrace _disposedAt;

  /// `true` if the icon has been constructed in native code.
  /// This is deferred until usage, because constructors can't be `async`.
  bool _isReal = false;

  /// Creates a new [TrayIcon] that controls a single icon in the system tray.
  TrayIcon() : _id = _newId() {
    _allIcons[_id] = this;
    _isActive = true;
    // In Debug mode users can hot restart the app.
    // [As of right now, there is no way to detect that.](https://github.com/flutter/flutter/issues/10437)
    // That means we have to call an init method for cleanup.
    // This method is automatically called, when the [BetrayalPlugin._instance]
    // is constructed.
    // This happens happens the first time [_plugin] is accessed.
    if (kDebugMode) _plugin;
  }

  /// Retrieve the [TrayIcon] managed by a [TrayIconWidget] further up the tree.
  static TrayIcon of(BuildContext context) {
    final TrayIcon? result =
        context.dependOnInheritedWidgetOfExactType<_TrayIconHeritage>()?.icon;
    assert(result != null, 'No TrayIcon found in context');
    return result!;
  }

  /// Disposes all icons and clears up any residual icons.
  ///
  /// Use this method to clean up after a hot restart,
  /// if you aren't going to immediately create a new [TrayIcon].
  ///
  /// ```dart
  /// void main() {
  ///   TrayIcon.clearAll();
  ///   runApp(const MyApp());
  /// }
  /// ```
  static void clearAll() {
    if (kDebugMode) _allIcons.clear();
    _plugin.reset();
  }

  /// Disposes the icon.
  ///
  /// This will permanently remove the icon from the system tray.
  /// Resources in native code will be released and this instance
  /// will become unusable.
  Future<void> dispose() async {
    _isActive = false;
    if (_isReal) {
      await _plugin.disposeTray(_id);
    }
    _allIcons.remove(_id);

    _disposedAt = StackTrace.current;
  }

  /// Ensures the icon has been constructed in native code.
  ///
  /// This is deferred until usage, because constructors can't be `async`.
  Future<void> _makeRealIfNeeded() async {
    if (!_isReal) {
      await _plugin.addTray(_id);
      _isReal = true;
    }
  }

  /// Tests if this instance has already been disposed of.
  void _ensureIsActive() {
    if (!_isActive) {
      throw StateError(
          'TrayIcon is not active anymore.\n\nIt was disposed at:\n${_disposedAt.toString()}\nCurrent StackTrace:');
    }
  }

  /// Shows the icon in the system tray.
  Future<void> show() async {
    _ensureIsActive();
    if (_isVisible) return;
    await _makeRealIfNeeded();
    await _plugin.showIcon(_id);
    _isVisible = true;
  }

  /// Hides the icon from the system tray.
  ///
  /// Internally the icon is actually unregistered from the system entirely,
  /// however this plugin still keeps track of it to be able to show it again.
  Future<void> hide() async {
    _ensureIsActive();
    if (!_isVisible) return;
    if (!_isReal) return;

    await _plugin.hideIcon(_id);
    _isVisible = false;
  }

  /// Sets the tooltip text. If [message] is `null`, the tooltip is removed.
  Future<void> setTooltip(String? message) async {
    _ensureIsActive();
    await _makeRealIfNeeded();
    if (message != null) {
      await _plugin.setTooltip(_id, message);
    } else {
      await _plugin.removeTooltip(_id);
    }
  }

  /// Sets the image on this icon.
  ///
  /// If multiple arguments are passed, they are resolved in this order:
  /// 1. [delegate]
  /// 2. [pixels]
  /// 3. [path]
  /// 4. [asset]
  /// 5. [winIcon]
  ///
  /// If no argument is passed, the image is removed.
  Future<void> setImage({
    TrayIconImageDelegate? delegate,
    Uri? path,
    ByteBuffer? pixels,
    String? asset,
    WinIcon? winIcon,
  }) async {
    _ensureIsActive();
    await _makeRealIfNeeded();

    if (delegate != null) {
    } else if (pixels != null) {
      delegate = TrayIconImageDelegate.fromBytes(pixels);
    } else if (asset != null) {
      delegate = TrayIconImageDelegate.fromAsset(asset);
    } else if (path != null) {
      delegate = TrayIconImageDelegate.fromPath(uri: path);
    } else if (winIcon != null) {
      delegate = TrayIconImageDelegate.fromWinIcon(winIcon);
    } else {
      delegate = TrayIconImageDelegate.noImage();
    }
    await delegate.setIcon(_id, _plugin);
  }
}
