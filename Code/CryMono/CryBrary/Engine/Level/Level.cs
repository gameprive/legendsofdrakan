﻿using System;
using System.Collections.Generic;

using System.Linq;

using System.Runtime.CompilerServices;
using CryEngine.Native;

namespace CryEngine
{
    /// <summary>
    /// Represents a CryENGINE level.
    /// </summary>
    public class Level
    {
        private static Dictionary<string, Level> levels;

        internal Level(IntPtr ptr)
        {
            Handle = ptr;

            levels = new Dictionary<string, Level>();
        }

        /// <summary>
        /// Gets the currently loaded level
        /// </summary>
        public static Level Current
        {
            get
            {
                var ptr = NativeLevelMethods.GetCurrentLevel();
                if (ptr != IntPtr.Zero)
                {
                    if (levels.Any(x => x.Value.Handle == ptr))
                    {
                        // TODO
                    }
                    else
                        return new Level(ptr);
                }

                return null;
            }
        }

        /// <summary>
        /// Gets a value indicating whether a level is currently loaded.
        /// </summary>
        public static bool Loaded { get { return NativeLevelMethods.IsLevelLoaded(); } }

        /// <summary>
        /// Gets the level name.
        /// </summary>
        public string Name { get { return NativeLevelMethods.GetName(Handle); } }

        /// <summary>
        /// Gets the level display name.
        /// </summary>
        public string DisplayName { get { return NativeLevelMethods.GetDisplayName(Handle); } }

        /// <summary>
        /// Gets the full path to the directory this level resides in.
        /// </summary>
        public string Path { get { return NativeLevelMethods.GetName(Handle); } }

        /// <summary>
        /// Gets the heightmap size for this level.
        /// </summary>
        public int HeightmapSize { get { return NativeLevelMethods.GetHeightmapSize(Handle); } }

        /// <summary>
        /// Gets the number of supported game rules for this level.
        /// </summary>
        public int SupportedGamerules { get { return NativeLevelMethods.GetGameTypeCount(Handle); } }

        /// <summary>
        /// Gets the default gamemode for this level.
        /// </summary>
        public string DefaultGameRules { get { return NativeLevelMethods.GetDefaultGameType(Handle); } }

        /// <summary>
        /// Gets a value indicating whether the level is configured to support any game rules.
        /// </summary>
        public bool HasGameRules { get { return NativeLevelMethods.HasGameRules(Handle); } }

        internal IntPtr Handle { get; set; }

        #region Statics
        /// <summary>
        /// Loads a new level and returns its level info
        /// </summary>
        /// <param name="name"></param>
        /// <returns>The loaded level</returns>
        public static Level Load(string name)
        {
            Level level;
            if (levels.TryGetValue(name, out level))
                return level;

            var ptr = NativeLevelMethods.LoadLevel(name);
            if (ptr != IntPtr.Zero)
                return new Level(ptr);

            return null;
        }

        /// <summary>
        /// Unloads the currently loaded level.
        /// </summary>
        public static void Unload()
        {
            NativeLevelMethods.UnloadLevel();
        }
        #endregion

        #region Overrides
        public override bool Equals(object obj)
        {
            if (obj != null && obj is Level)
                return this == obj;

            return false;
        }

        public override int GetHashCode()
        {
            unchecked // Overflow is fine, just wrap
            {
                int hash = 17;

                hash = hash * 29 + Handle.GetHashCode();

                return hash;
            }
        }
        #endregion

        /// <summary>
        /// Gets the supported game rules at the index; see SupportedGamerules.
        /// </summary>
        /// <param name="index"></param>
        /// <returns>Name of the supported gamemode</returns>
        public string GetSupportedGameRules(int index)
        {
            return NativeLevelMethods.GetGameType(Handle, index);
        }

        /// <summary>
        /// Returns true if this level supports the specific game rules.
        /// </summary>
        /// <param name="gamemodeName"></param>
        /// <returns>A boolean indicating whether the specified gamemode is supported.</returns>
        public bool SupportsGameRules(string gamemodeName)
        {
            return NativeLevelMethods.SupportsGameType(Handle, gamemodeName);
        }
    }
}
