/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include <QTest>

class ClipboardSyncTests : public QObject
{
  Q_OBJECT
private Q_SLOTS:
  void formatFilter_allEnabled_passesAllFormats();
  void formatFilter_textOnly_stripsHTMLAndBitmap();
  void formatFilter_bitmapDisabled_stripsBitmapKeepsRest();
  void formatFilter_noneEnabled_producesEmptyClipboard();
  void formatFilter_htmlOnly_stripsTextAndBitmap();

  void direction_bidirectional_defaultValue();
  void direction_serverToClient_constant();
  void direction_clientToServer_constant();

  void optionConstants_clipboardFormatsAll_equals7();
  void optionConstants_individualBits_distinct();
};
