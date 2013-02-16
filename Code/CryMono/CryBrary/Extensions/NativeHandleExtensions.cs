﻿using System;

using CryEngine;
using CryEngine.Utilities;

namespace CryEngine.Native
{
    public static class NativeHandleExtensions
    {
        #region Actor
        public static IntPtr GetIActor(this ActorBase actor)
        {
            if (actor.IsDestroyed)
                throw new ScriptInstanceDestroyedException("Attempted to access native actor handle on a destroyed script");

            return actor.ActorHandle;
        }

        public static void SetIActor(this ActorBase actor, IntPtr handle)
        {
            actor.ActorHandle = handle;
        }
        #endregion

        #region Attachment
        public static IntPtr GetIAttachment(this Attachment attachment)
        {
            if (attachment.IsDestroyed)
                throw new ScriptInstanceDestroyedException("Attempted to access native attachment handle on a destroyed script");

            return attachment.AttachmentHandle;
        }

        public static void SetIAttachment(this Attachment attachment, IntPtr newHandle)
        {
            attachment.AttachmentHandle = newHandle;
        }
        #endregion

        #region Entity
        public static IntPtr GetIEntity(this EntityBase entity)
        {
            if (entity.IsDestroyed)
                throw new ScriptInstanceDestroyedException("Attempted to access native entity handle on a destroyed script");

            return entity.EntityHandle;
        }

        public static void SetIEntity(this EntityBase entity, IntPtr handle)
        {
            entity.EntityHandle = handle;
        }

        public static IntPtr GetIEntityLink(this EntityLink link)
        {
            return link.Handle;
        }

        public static void SetIEntityLink(this EntityLink link, IntPtr handle)
        {
            link.Handle = handle;
        }

        public static IntPtr GetIAnimatedCharacter(this EntityBase entity)
        {
            if (entity.IsDestroyed)
                throw new ScriptInstanceDestroyedException("Attempted to access native animated character handle on a destroyed entity");
            if (entity.AnimatedCharacterHandle == IntPtr.Zero)
                entity.SetIAnimatedCharacter(NativeEntityMethods.AcquireAnimatedCharacter(entity.Id));

            return entity.AnimatedCharacterHandle;
        }

        public static void SetIAnimatedCharacter(this EntityBase entity, IntPtr handle)
        {
            entity.AnimatedCharacterHandle = handle;
        }
        #endregion

        #region Material
        public static IntPtr GetIMaterial(this Material mat)
        {
            return mat.Handle;
        }

        public static void SetIMaterial(this Material mat, IntPtr handle)
        {
            mat.Handle = handle;
        }
        #endregion

        #region Lua ScriptTable
        public static IntPtr GetIScriptTable(this Lua.ScriptTable scriptTable)
        {
            return scriptTable.Handle;
        }

        public static void SetIScriptTable(this Lua.ScriptTable scriptTable, IntPtr handle)
        {
            scriptTable.Handle = handle;
        }
        #endregion

        #region SurfaceType
        public static IntPtr GetISurfaceType(this SurfaceType surfaceType)
        {
            return surfaceType.Handle;
        }

        public static void SetISurfaceType(this SurfaceType surfaceType, IntPtr handle)
        {
            surfaceType.Handle = handle;
        }
        #endregion

        #region CrySerialize
        public static IntPtr GetISerialize(this Serialization.CrySerialize serialize)
        {
            return serialize.Handle;
        }

        public static void SetISerialize(this Serialization.CrySerialize serialize, IntPtr handle)
        {
            serialize.Handle = handle;
        }
        #endregion

        #region FlowNode
        public static IntPtr GetIFlowNode(this FlowSystem.FlowNode node)
        {
            return node.Handle;
        }

        public static void SetIFlowNode(this FlowSystem.FlowNode node, IntPtr handle)
        {
            node.Handle = handle;
        }
        #endregion

        #region Physical Entity
        public static IntPtr GetIPhysicalEntity(this Physics.PhysicalEntity physicalEntity)
        {
            return physicalEntity.Handle;
        }

        public static void SetIPhysicalEntity(this Physics.PhysicalEntity physicalEntity, IntPtr handle)
        {
            physicalEntity.Handle = handle;
        }
        #endregion
    }
}
