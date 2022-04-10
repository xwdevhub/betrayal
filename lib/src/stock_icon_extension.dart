part of 'stock_icon.dart';

/// This extension allows adding members to the [StockIcon] enum.
///
/// In Dart 2.17 and later, this will be possible doable
/// without an extension.
extension StockIconCodes on StockIcon {
  /// The internal `SHSTOCKICONID` used by Windows.
  int get code {
    switch (this) {
      case StockIcon.documentOfUnknownType:
        return 0;
      case StockIcon.document:
        return 1;
      case StockIcon.application:
        return 2;
      case StockIcon.folder:
        return 3;
      case StockIcon.folderOpen:
        return 4;
      case StockIcon.drive525:
        return 5;
      case StockIcon.drive35:
        return 6;
      case StockIcon.driveRemovable:
        return 7;
      case StockIcon.driveFixed:
        return 8;
      case StockIcon.driveNetwork:
        return 9;
      case StockIcon.driveNetworkDisconnected:
        return 10;
      case StockIcon.driveCd:
        return 11;
      case StockIcon.ramDisk:
        return 12;
      case StockIcon.networkLocal:
        return 13;
      case StockIcon.server:
        return 15;
      case StockIcon.printer:
        return 16;
      case StockIcon.network:
        return 17;
      case StockIcon.search:
        return 22;
      case StockIcon.help:
        return 23;
      case StockIcon.shared:
        return 28;
      case StockIcon.shortcut:
        return 29;
      case StockIcon.slowFile:
        return 30;
      case StockIcon.trash:
        return 31;
      case StockIcon.trashFull:
        return 32;
      case StockIcon.cdAudio:
        return 40;
      case StockIcon.lock:
        return 47;
      case StockIcon.searchFolder:
        return 49;
      case StockIcon.printerNetwork:
        return 50;
      case StockIcon.folderNetwork:
        return 51;
      case StockIcon.faxPrinter:
        return 52;
      case StockIcon.faxPrinterNetwork:
        return 53;
      case StockIcon.printToFile:
        return 54;
      case StockIcon.stack:
        return 55;
      case StockIcon.superVideoCd:
        return 56;
      case StockIcon.folderStuffed:
        return 57;
      case StockIcon.driveUnknown:
        return 58;
      case StockIcon.driveDvd:
        return 59;
      case StockIcon.dvd:
        return 60;
      case StockIcon.dvdRam:
        return 61;
      case StockIcon.dvdRw:
        return 62;
      case StockIcon.dvdR:
        return 63;
      case StockIcon.dvdRom:
        return 64;
      case StockIcon.cdPlus:
        return 65;
      case StockIcon.cdRw:
        return 66;
      case StockIcon.cdR:
        return 67;
      case StockIcon.cdBurn:
        return 68;
      case StockIcon.cdBlank:
        return 69;
      case StockIcon.cdRom:
        return 70;
      case StockIcon.audioFile:
        return 71;
      case StockIcon.imageFile:
        return 72;
      case StockIcon.videoFile:
        return 73;
      case StockIcon.mixedFile:
        return 74;
      case StockIcon.folderBack:
        return 75;
      case StockIcon.folderFront:
        return 76;
      case StockIcon.shield:
        return 77;
      case StockIcon.warning:
        return 78;
      case StockIcon.info:
        return 79;
      case StockIcon.error:
        return 80;
      case StockIcon.key:
        return 81;
      case StockIcon.software:
        return 82;
      case StockIcon.rename:
        return 83;
      case StockIcon.delete:
        return 84;
      case StockIcon.audioDvd:
        return 85;
      case StockIcon.movieDvd:
        return 86;
      case StockIcon.enhancedCd:
        return 87;
      case StockIcon.enhancedDvd:
        return 88;
      case StockIcon.hdDvd:
        return 89;
      case StockIcon.bluRay:
        return 90;
      case StockIcon.videoCd:
        return 91;
      case StockIcon.dvdPlusR:
        return 92;
      case StockIcon.dvdPlusRw:
        return 93;
      case StockIcon.desktopComputer:
        return 94;
      case StockIcon.laptop:
        return 95;
      case StockIcon.users:
        return 96;
      case StockIcon.smartMedia:
        return 97;
      case StockIcon.compactFlash:
        return 98;
      case StockIcon.cellPhone:
        return 99;
      case StockIcon.camera:
        return 100;
      case StockIcon.videoCamera:
        return 101;
      case StockIcon.audioPlayer:
        return 102;
      case StockIcon.networkConnect:
        return 103;
      case StockIcon.internet:
        return 104;
      case StockIcon.zipFile:
        return 105;
      case StockIcon.settings:
        return 106;
      case StockIcon.driveHdDvd:
        return 132;
      case StockIcon.driveBd:
        return 133;
      case StockIcon.hdDvdRom:
        return 134;
      case StockIcon.hdDvdR:
        return 135;
      case StockIcon.hdDvdRam:
        return 136;
      case StockIcon.bdRom:
        return 137;
      case StockIcon.bdR:
        return 138;
      case StockIcon.bdRe:
        return 139;
      case StockIcon.clusteredDrive:
        return 140;
    }
  }

  /// Returns a combination of emojis that look similar the actual [StockIcon].
  ///
  /// 💫 is used as a fallback value.
  String get emojis {
    switch (this) {
      case StockIcon.documentOfUnknownType:
      case StockIcon.document:
        return '📄';
      case StockIcon.application:
        return '📰';
      case StockIcon.folder:
        return '📁';
      case StockIcon.folderOpen:
        return '📂';
      case StockIcon.drive525:
      case StockIcon.drive35:
        return '🖴💾';
      case StockIcon.driveRemovable:
      case StockIcon.driveFixed:
        return '🖴';
      case StockIcon.driveNetwork:
        return '🖴🔌';
      case StockIcon.driveNetworkDisconnected:
        return '🖴❌';
      case StockIcon.driveCd:
        return '🖴💿';
      case StockIcon.ramDisk:
        return '🕷️';
      case StockIcon.networkLocal:
        return '🖥️🔗🖥️';
      case StockIcon.server:
        return '🖥️';
      case StockIcon.printer:
        return '🖨️';
      case StockIcon.network:
        return '🌍🖥️';
      case StockIcon.search:
        return '🔎';
      case StockIcon.help:
        return 'ℹ️';
      case StockIcon.shared:
        return '👨🏽‍🤝‍👨🏿';
      case StockIcon.shortcut:
        return '⤴';
      case StockIcon.slowFile:
        return '✖️';
      case StockIcon.trash:
      case StockIcon.trashFull:
        return '🗑️';
      case StockIcon.cdAudio:
        return '💿🎶';
      case StockIcon.lock:
        return '🔒';
      case StockIcon.searchFolder:
        return '📁🔎';
      case StockIcon.printerNetwork:
        return '🖨️🔌';
      case StockIcon.folderNetwork:
        return '📁🔌';
      case StockIcon.faxPrinter:
        return '🖨️';
      case StockIcon.faxPrinterNetwork:
        return '🖨️🔌';
      case StockIcon.printToFile:
        return '🖨️💾';
      case StockIcon.stack:
        return '🗂️';
      case StockIcon.superVideoCd:
        return '💿🎞️';
      case StockIcon.folderStuffed:
        return '📁🥨';
      case StockIcon.driveUnknown:
        return '🖴❔';
      case StockIcon.driveDvd:
        return '🖴💿';
      case StockIcon.dvd:
      case StockIcon.dvdRam:
        return '💿';
      case StockIcon.dvdRw:
        return '💿🖊️';
      case StockIcon.dvdR:
      case StockIcon.dvdRom:
        return '💿👁️‍🗨️';
      case StockIcon.cdPlus:
        return '💽➕🎶';
      case StockIcon.cdRw:
        return '💽🖊️';
      case StockIcon.cdR:
        return '💽👁️‍🗨️';
      case StockIcon.cdBurn:
        return '💽🔥';
      case StockIcon.cdBlank:
        return '💽✨';
      case StockIcon.cdRom:
        return '💽👁️‍🗨️';
      case StockIcon.audioFile:
        return '🎵📄';
      case StockIcon.imageFile:
        return '🖼️📄';
      case StockIcon.videoFile:
        return '🎞️📄';
      case StockIcon.mixedFile:
        return '🎞️🎵📄';
      case StockIcon.folderBack:
        return '📂🔙';
      case StockIcon.folderFront:
        return '📂';
      case StockIcon.shield:
        return '🛡️';
      case StockIcon.warning:
        return '⚠️';
      case StockIcon.info:
        return 'ℹ️';
      case StockIcon.error:
        return '❌';
      case StockIcon.key:
        return '🔑';
      case StockIcon.software:
        return '📦';
      case StockIcon.rename:
        return '🖊️';
      case StockIcon.delete:
        return '❌';
      case StockIcon.audioDvd:
        return '💿🎶';
      case StockIcon.movieDvd:
        return '💿🎞️';
      case StockIcon.enhancedCd:
        return '💽🎶';
      case StockIcon.enhancedDvd:
      case StockIcon.hdDvd:
      case StockIcon.bluRay:
        return '💿🎞️';
      case StockIcon.videoCd:
        return '💽🎞️';
      case StockIcon.dvdPlusR:
        return '💿👁️‍🗨️';
      case StockIcon.dvdPlusRw:
        return '💿🖊️';
      case StockIcon.desktopComputer:
        return '🖥️';
      case StockIcon.laptop:
        return '💻';
      case StockIcon.users:
        return '👨‍👩‍👧';
      case StockIcon.smartMedia:
        return '💾🧠';
      case StockIcon.compactFlash:
        return '☄️';
      case StockIcon.cellPhone:
        return '📱';
      case StockIcon.camera:
        return '📷';
      case StockIcon.videoCamera:
        return '📹';
      case StockIcon.audioPlayer:
        return '📻';
      case StockIcon.networkConnect:
        return '🖥️↔️🖥️';
      case StockIcon.internet:
        return '🌐';
      case StockIcon.zipFile:
        return '🗜️📁';
      case StockIcon.settings:
        return '⚙️';
      case StockIcon.driveHdDvd:
      case StockIcon.driveBd:
        return '🖴💿';
      case StockIcon.hdDvdRom:
        return '💿👁️‍🗨️';
      case StockIcon.hdDvdRam:
        return '💿';
      case StockIcon.bdRom:
        return '💿👁️‍🗨️';
      case StockIcon.bdR:
        return '💿👁️‍🗨️';
      case StockIcon.bdRe:
        return '💿👁️‍🗨️';
      case StockIcon.clusteredDrive:
        return '🖴';
      default:
        return '💫';
    }
  }

  /// Returns a emoji that looks similar to the actual [StockIcon].
  ///
  /// 💫 is used as a fallback value.
  String get emoji => String.fromCharCode(emojis.runes.first);
}