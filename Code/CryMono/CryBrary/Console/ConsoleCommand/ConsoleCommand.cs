﻿using System;
using System.Collections.Generic;

using CryEngine.Native;

namespace CryEngine
{
    public delegate void ConsoleCommandDelegate(ConsoleCommandArgs e);

    public static class ConsoleCommand
    {
        static Dictionary<string, ConsoleCommandDelegate> commands = new Dictionary<string, ConsoleCommandDelegate>();

        /// <summary>
        /// Executes a string in the console
        /// </summary>
        /// <param name="command">console command e.g. "map testy" - no leading slash</param>
        /// <param name="silent">suppresses log in error case and logging the command to the console</param>
        public static void Execute(string command, bool silent = false)
        {
            NativeCVarMethods.Execute(command, silent);
        }

        /// <summary>
        /// Register a new console command.
        /// </summary>
        /// <param name="name">Command name.</param>
        /// <param name="func">Delegate to the console command function to be called when command is invoked.</param>
        /// <param name="comment">Help string, will be displayed when typing in console "command ?".</param>
        /// <param name="flags">Bitfield consist of VF_ flags (e.g. VF_CHEAT)</param>
        public static void Register(string name, ConsoleCommandDelegate func, string comment = "", CVarFlags flags = CVarFlags.None)
        {
            if (!commands.ContainsKey(name))
            {
                NativeCVarMethods.RegisterCommand(name, comment, flags);

                commands.Add(name, func);
            }
        }

        internal static void OnCommand(string fullCommandLine)
        {
            var argsWithName = fullCommandLine.Split(' ');
            var name = argsWithName[0];

            var args = new string[argsWithName.Length - 1];
            for (int i = 1; i < argsWithName.Length; i++)
                args[i - 1] = argsWithName[i];

            commands[name](new ConsoleCommandArgs(name, args, fullCommandLine));
        }
    }
}
