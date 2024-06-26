﻿#pragma once

#include <vector>
#include <random>

#include <QString>
#include <QApplication>

#include "KContext.h"
#include <AIS_ColoredShape.hxx>

namespace KDebugger
{
extern KContext *G_Context;

extern std::default_random_engine G_Random_Engine;

extern bool G_Stop_Program; // exit circulation

bool GCheckCpuMode();
unsigned long long G_CoreCount(unsigned long long datasize,
                               unsigned long long min_per_thread = 5000);

enum class Appearance
{
    Dark,
    Day,
    System
};
extern Appearance G_UI_Appearance;

Appearance getUIConfig();
QString G_GetUiStyleSheet(Appearance preset);
QString G_GetAppPath();
QString G_GetDocumentsPath();
} // namespace KDebugger