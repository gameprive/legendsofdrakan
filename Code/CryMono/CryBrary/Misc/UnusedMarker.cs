﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;

namespace CryEngine.Utilities
{
    public static class UnusedMarker
    {
        [StructLayout(LayoutKind.Explicit)]
        struct f2i
        {
            [FieldOffset(0)]
            public float f;

            [FieldOffset(0)]
            public UInt32 i;
        }

        [StructLayout(LayoutKind.Explicit)]
        struct d2i
        {
            [FieldOffset(0)]
            public double d;

            [FieldOffset(0)]
            public UInt32 i;
        }

        public static float Float
        {
            get
            {
                var u = new f2i();
                u.i = 0xFFBFFFFF;

                return u.f;
            }
        }

        public static bool IsUnused(float var)
        {
            var u = new f2i();
            u.f = var;
            return (u.i & 0xFFA00000) == 0xFFA00000;
        }

        public static int Integer
        {
            get
            {
                return 1 << 31;
            }
        }

        public static bool IsUnused(int var)
        {
            return var == 1 << 31;
        }

        [CLSCompliant(false)]
        public static uint UnsignedInteger
        {
            get
            {
                return 1u << 31;
            }
        }

        [CLSCompliant(false)]
        public static bool IsUnused(uint var)
        {
            return var == 1u << 31;
        }

        public static Vec3 Vec3
        {
            get
            {
                return new Vec3(Float);
            }
        }

        public static bool IsUnused(Vec3 var)
        {
            return IsUnused(var.X);
        }

        public static IntPtr IntPtr
        {
            get
            {
                return new IntPtr(-1);
            }
        }

        public static bool IsUnused(IntPtr var)
        {
            return var.ToInt32() == -1;
        }
    }
}