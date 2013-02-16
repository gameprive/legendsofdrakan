﻿using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using CryEngine.Initialization;
using CryEngine.Extensions;
using CryEngine.Native;

namespace CryEngine
{
    /// <summary>
    /// This is the base GameRules interface. All game rules must implement this.
    /// </summary>
    public abstract class GameRules : EntityBase
    {
        internal void InternalInitialize()
        {
            Current = this;

            Id = 1;
            this.SetIEntity(NativeEntityMethods.GetEntity(Id));
        }

        /// <summary>
        /// Gets the currently active game rules instance.
        /// </summary>
        public static GameRules Current { get; internal set; }
    }
}