﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CryEngine.Initialization
{
    public enum ScriptReloadResult
    {
        Success,
        Retry,
        Revert,
        Abort
    }
}
