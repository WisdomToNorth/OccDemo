#pragma once

#include <AIS_InteractiveContext.hxx>
#include <AIS_ColoredShape.hxx>
#include <vector>
#include <QString>
#include <QApplication>
namespace KDebugger
{
extern Handle(AIS_InteractiveContext) G_Context;

extern bool G_Stop_Program;//exit circulation
bool GCheckCpuMode();
unsigned long long G_CoreCount(unsigned long long datasize);

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
}