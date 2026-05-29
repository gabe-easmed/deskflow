/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "ClipboardSyncTests.h"

#include "deskflow/Clipboard.h"
#include "deskflow/IClipboard.h"
#include "deskflow/OptionTypes.h"

// Mirror of the static helper in Server.cpp so we can unit-test it independently.
static Clipboard makeFilteredClipboard(const Clipboard &src, uint32_t formats)
{
  using Format = IClipboard::Format;
  Clipboard filtered;
  filtered.open(0);
  filtered.empty();
  src.open(0);
  if ((formats & kClipboardFormatText) && src.has(Format::Text))
    filtered.add(Format::Text, src.get(Format::Text));
  if ((formats & kClipboardFormatHTML) && src.has(Format::HTML))
    filtered.add(Format::HTML, src.get(Format::HTML));
  if ((formats & kClipboardFormatBitmap) && src.has(Format::Bitmap))
    filtered.add(Format::Bitmap, src.get(Format::Bitmap));
  src.close();
  filtered.close();
  return filtered;
}

// Build a source clipboard with all three formats populated.
static Clipboard makeFullClipboard()
{
  Clipboard cb;
  cb.open(0);
  cb.empty();
  cb.add(IClipboard::Format::Text,   "hello text");
  cb.add(IClipboard::Format::HTML,   "<b>hello</b>");
  cb.add(IClipboard::Format::Bitmap, "BMPDATA");
  cb.close();
  return cb;
}

// ── Format filter tests ──────────────────────────────────────────────────────

void ClipboardSyncTests::formatFilter_allEnabled_passesAllFormats()
{
  auto src = makeFullClipboard();
  auto out = makeFilteredClipboard(src, kClipboardFormatsAll);

  out.open(0);
  QVERIFY(out.has(IClipboard::Format::Text));
  QVERIFY(out.has(IClipboard::Format::HTML));
  QVERIFY(out.has(IClipboard::Format::Bitmap));
  QCOMPARE(out.get(IClipboard::Format::Text),   std::string("hello text"));
  QCOMPARE(out.get(IClipboard::Format::HTML),   std::string("<b>hello</b>"));
  QCOMPARE(out.get(IClipboard::Format::Bitmap), std::string("BMPDATA"));
  out.close();
}

void ClipboardSyncTests::formatFilter_textOnly_stripsHTMLAndBitmap()
{
  auto src = makeFullClipboard();
  auto out = makeFilteredClipboard(src, kClipboardFormatText);

  out.open(0);
  QVERIFY(out.has(IClipboard::Format::Text));
  QVERIFY(!out.has(IClipboard::Format::HTML));
  QVERIFY(!out.has(IClipboard::Format::Bitmap));
  out.close();
}

void ClipboardSyncTests::formatFilter_bitmapDisabled_stripsBitmapKeepsRest()
{
  auto src = makeFullClipboard();
  const uint32_t nobitmap = kClipboardFormatText | kClipboardFormatHTML;
  auto out = makeFilteredClipboard(src, nobitmap);

  out.open(0);
  QVERIFY(out.has(IClipboard::Format::Text));
  QVERIFY(out.has(IClipboard::Format::HTML));
  QVERIFY(!out.has(IClipboard::Format::Bitmap));
  out.close();
}

void ClipboardSyncTests::formatFilter_noneEnabled_producesEmptyClipboard()
{
  auto src = makeFullClipboard();
  auto out = makeFilteredClipboard(src, 0u);

  out.open(0);
  QVERIFY(!out.has(IClipboard::Format::Text));
  QVERIFY(!out.has(IClipboard::Format::HTML));
  QVERIFY(!out.has(IClipboard::Format::Bitmap));
  out.close();
}

void ClipboardSyncTests::formatFilter_htmlOnly_stripsTextAndBitmap()
{
  auto src = makeFullClipboard();
  auto out = makeFilteredClipboard(src, kClipboardFormatHTML);

  out.open(0);
  QVERIFY(!out.has(IClipboard::Format::Text));
  QVERIFY(out.has(IClipboard::Format::HTML));
  QVERIFY(!out.has(IClipboard::Format::Bitmap));
  out.close();
}

// ── Direction constant tests ─────────────────────────────────────────────────

void ClipboardSyncTests::direction_bidirectional_defaultValue()
{
  QCOMPARE(kClipboardDirectionBidirectional, 0);
}

void ClipboardSyncTests::direction_serverToClient_constant()
{
  QCOMPARE(kClipboardDirectionServerToClient, 1);
}

void ClipboardSyncTests::direction_clientToServer_constant()
{
  QCOMPARE(kClipboardDirectionClientToServer, 2);
}

// ── Option constant tests ────────────────────────────────────────────────────

void ClipboardSyncTests::optionConstants_clipboardFormatsAll_equals7()
{
  QCOMPARE(kClipboardFormatsAll, 7);
}

void ClipboardSyncTests::optionConstants_individualBits_distinct()
{
  QVERIFY((kClipboardFormatText & kClipboardFormatHTML)   == 0);
  QVERIFY((kClipboardFormatText & kClipboardFormatBitmap) == 0);
  QVERIFY((kClipboardFormatHTML & kClipboardFormatBitmap) == 0);
  QCOMPARE(kClipboardFormatText | kClipboardFormatHTML | kClipboardFormatBitmap, kClipboardFormatsAll);
}

QTEST_MAIN(ClipboardSyncTests)
