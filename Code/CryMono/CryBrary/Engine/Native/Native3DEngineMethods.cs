﻿using System;
using System.Runtime.CompilerServices;

namespace CryEngine.Native
{
    internal static class Native3DEngineMethods
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern internal static float GetTerrainElevation(float positionX, float positionY, bool includeOutdoorVoxels);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static int GetTerrainSize();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static int GetTerrainSectorSize();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static int GetTerrainUnitSize();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static int GetTerrainZ(int x, int y);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void SetTimeOfDay(float hour, bool forceUpdate = false);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static float GetTimeOfDay();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static TimeOfDay.AdvancedInfo GetTimeOfDayAdvancedInfo();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void SetTimeOfDayAdvancedInfo(TimeOfDay.AdvancedInfo advancedInfo);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void SetTimeOfDayVariableValue(int id, float value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void SetTimeOfDayVariableValueColor(int id, Vec3 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void ActivatePortal(Vec3 pos, bool activate, string entityName);
    }
}
