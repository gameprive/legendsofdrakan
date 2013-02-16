﻿using System;
using System.Linq;

using System.Collections.Generic;

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using CryEngine.Initialization;
using CryEngine.Native;
using CryEngine.Physics;

namespace CryEngine
{
    /// <summary>
    /// Represents an CryENGINE entity
    /// </summary>
    public abstract class EntityBase : CryScriptInstance
    {
        internal PhysicalEntity _physics;

        /// <summary>
        /// Gets this entity's Lua script table, providing it exists.
        /// </summary>
        public Lua.ScriptTable ScriptTable { get { return Lua.ScriptTable.Get(this.GetIEntity()); } }

        /// <summary>
        /// Gets or sets a value indicating whether the entity is hidden or not.
        /// </summary>
        public bool Hidden { get { return NativeEntityMethods.IsHidden(this.GetIEntity()); } set { NativeEntityMethods.Hide(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the world space entity position.
        /// </summary>
        public Vec3 Position { get { return NativeEntityMethods.GetWorldPos(this.GetIEntity()); } set { NativeEntityMethods.SetWorldPos(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the world space entity orientation quaternion.
        /// </summary>
        public Quat Rotation { get { return NativeEntityMethods.GetWorldRotation(this.GetIEntity()); } set { NativeEntityMethods.SetWorldRotation(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the local space entity position.
        /// </summary>
        public Vec3 LocalPosition { get { return NativeEntityMethods.GetPos(this.GetIEntity()); } set { NativeEntityMethods.SetPos(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the local space entity orientation quaternion.
        /// </summary>
        public Quat LocalRotation { get { return NativeEntityMethods.GetRotation(this.GetIEntity()); } set { NativeEntityMethods.SetRotation(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the world space entity transformation matrix.
        /// </summary>
        public Matrix34 Transform { get { return NativeEntityMethods.GetWorldTM(this.GetIEntity()); } set { NativeEntityMethods.SetWorldTM(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the local space entity transformation matrix.
        /// </summary>
        public Matrix34 LocalTransform { get { return NativeEntityMethods.GetLocalTM(this.GetIEntity()); } set { NativeEntityMethods.SetLocalTM(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets the entity axis aligned bounding box in the world space.
        /// </summary>
        public BoundingBox BoundingBox { get { return NativeEntityMethods.GetWorldBoundingBox(this.GetIEntity()); } }

        /// <summary>
        /// Gets the entity axis aligned bounding box in the world space.
        /// </summary>
        public BoundingBox LocalBoundingBox { get { return NativeEntityMethods.GetBoundingBox(this.GetIEntity()); } }

        /// <summary>
        /// Gets or sets the entity name.
        /// </summary>
        public string Name { get { return NativeEntityMethods.GetName(this.GetIEntity()); } set { NativeEntityMethods.SetName(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the entity flags.
        /// </summary>
        public EntityFlags Flags { get { return NativeEntityMethods.GetFlags(this.GetIEntity()); } set { NativeEntityMethods.SetFlags(this.GetIEntity(), value); } }

        /// <summary>
        /// Gets or sets the material currently assigned to this entity.
        /// </summary>
        public Material Material { get { return Material.Get(this); } set { Material.Set(this, value); } }

        /// <summary>
        /// Gets  the runtime unique identifier of this entity assigned to it by the Entity System.
        /// EntityId may not be the same when saving/loading entity.
        /// EntityId is mostly used in runtime for fast and unique identification of entities..
        /// </summary>
        public EntityId Id { get; internal set; }

        public EntityUpdatePolicy UpdatePolicy { get { return NativeEntityMethods.GetUpdatePolicy(this.GetIEntity()); } set { NativeEntityMethods.SetUpdatePolicy(this.GetIEntity(), value); } }

        public bool ReceivePrePhysicsUpdates
        {
            get { return NativeEntityMethods.IsPrePhysicsUpdateActive(this.GetIEntity()); }
            set { NativeEntityMethods.EnablePrePhysicsUpdate(this.GetIEntity(), value); }
        }

        /// <summary>
        /// Gets or sets entity velocity as set by the physics system.
        /// </summary>
        public Vec3 Velocity
        {
            get { return NativePhysicsMethods.GetVelocity(this.GetIEntity()); }
            set { NativePhysicsMethods.SetVelocity(this.GetIEntity(), value); }
        }

        /// <summary>
        /// Gets the physical entity, contains essential functions for modifying the entitys existing physical state.
        /// </summary>
        public PhysicalEntity Physics
        {
            get { return _physics ?? (_physics = new PhysicalEntity(this)); }
        }

        #region Native handles
        /// <summary>
        /// Gets or sets IEntity handle
        /// </summary>
        internal IntPtr EntityHandle { get; set; }

        /// <summary>
        /// Gets or sets IAnimatedCharacter handle
        /// </summary>
        internal IntPtr AnimatedCharacterHandle { get; set; }
        #endregion

        /// <summary>
        /// Removes the entity from the CryEngine world.
        /// </summary>
        /// <param name="forceRemoveNow">If true, the entity will be removed immediately.</param>
        public virtual void Remove(bool forceRemoveNow = false)
        {
            Entity.Remove(Id, forceRemoveNow);
        }

        /// <summary>
        /// Retrieves the flags of the specified slot.
        /// </summary>
        /// <param name="slot">Index of the slot</param>
        /// <returns>The slot flags, or 0 if specified slot is not valid.</returns>
        public EntitySlotFlags GetSlotFlags(int slot = 0)
        {
            return NativeEntityMethods.GetSlotFlags(this.GetIEntity(), slot);
        }

        /// <summary>
        /// Sets the flags of the specified slot.
        /// </summary>
        /// <param name="flags">Flags to set.</param>
        /// <param name="slot">Index of the slot, if -1 apply to all existing slots.</param>
        public void SetSlotFlags(EntitySlotFlags flags, int slot = 0)
        {
            NativeEntityMethods.SetSlotFlags(this.GetIEntity(), slot, flags);
        }

#region Attachments
        /// <summary>
        /// Gets the attachment at the specified slot and index.
        /// </summary>
        /// <param name="index">Attachment index</param>
        /// <param name="characterSlot">Index of the character slot we wish to get an attachment from</param>
        /// <returns>null if failed, otherwise the attachment.</returns>
        public Attachment GetAttachment(int index, int characterSlot = 0)
        {
            var ptr = NativeEntityMethods.GetAttachmentByIndex(this.GetIEntity(), index, characterSlot);
            if (ptr == IntPtr.Zero)
                return null;

            return Attachment.TryAdd(ptr, this);
        }

        /// <summary>
        /// Gets the attachment by name at the specified slot.
        /// </summary>
        /// <param name="name">Attachment name</param>
        /// <param name="characterSlot">Index of the character slot we wish to get an attachment from</param>
        /// <returns>null if failed, otherwise the attachment.</returns>
        public Attachment GetAttachment(string name, int characterSlot = 0)
        {
#if !(RELEASE && RELEASE_DISABLE_CHECKS)
            if (String.IsNullOrEmpty(name))
                throw new ArgumentNullException("name");
#endif

            var ptr = NativeEntityMethods.GetAttachmentByName(this.GetIEntity(), name, characterSlot);

            return Attachment.TryAdd(ptr, this);
        }

        /// <summary>
        /// Gets the number of attachments at the specified character slot.
        /// </summary>
        /// <param name="characterSlot">Index of the slot we wish to get the attachment count of</param>
        /// <returns>Number of attachments at the specified slot</returns>
        public int GetAttachmentCount(int characterSlot = 0)
        {
            return NativeEntityMethods.GetAttachmentCount(this.GetIEntity(), characterSlot); 
        }
#endregion

        public IEnumerable<EntityLink> Links
        {
            get
            {
                var links = NativeEntityMethods.GetEntityLinks(this.GetIEntity());
                if (links == null)
                    yield break;

                foreach (object obj in links)
                {
                    var linkPtr = (int)obj;

                    yield return new EntityLink(new IntPtr(linkPtr), this);
                }
            }
        }

        public void RemoveAllLinks()
        {
            EntityLink.RemoveAll(this);
        }

        /// <summary>
        /// Loads a light source to the specified slot, or to the next available slot.
        /// </summary>
        /// <param name="parameters">New params of the light source we wish to load</param>
        /// <param name="slot">Slot we want to load the light into, if -1 chooses the next available slot.</param>
        /// <returns>The slot where the light source was loaded, or -1 if loading failed.</returns>
        public int LoadLight(LightParams parameters, int slot = 1)
        {
            return NativeEntityMethods.LoadLight(this.GetIEntity(), slot, parameters);
        }

        /// <summary>
        /// Loads a mesh for this entity. Can optionally load multiple meshes using entity slots.
        /// </summary>
        /// <param name="name">Path to the object (Relative to the game directory)</param>
        /// <param name="slot"></param>
        /// <returns>true if successful, otherwise false.</returns>
        public bool LoadObject(string name, int slot = 0)
        {
            if (name == null)
                throw new ArgumentNullException("name");

            if (name.EndsWith("cgf"))
                NativeEntityMethods.LoadObject(this.GetIEntity(), name, slot);
            else if (name.EndsWith("cdf") || name.EndsWith("cga") || name.EndsWith("chr"))
                NativeEntityMethods.LoadCharacter(this.GetIEntity(), name, slot);
            else
                return false;

            return true;
        }

        /// <summary>
        /// Gets the path to the currently loaded object.
        /// </summary>
        /// <param name="slot">Slot containing the object we want to know the path of.</param>
        /// <returns>Path to the currently loaded object at the specified slot.</returns>
        public string GetObjectFilePath(int slot = 0)
        {
            return NativeEntityMethods.GetStaticObjectFilePath(this.GetIEntity(), slot);
        }

        /// <summary>
        /// Plays a raw animation.
        /// </summary>
        /// <param name="animationName">Name of the animation we wish to play</param>
        /// <param name="flags"></param>
        /// <param name="slot">Slot on which to play the animation</param>
        /// <param name="layer">Animation layer to play the animation in.</param>
        /// <param name="blend">Transition time between two animations.</param>
        /// <param name="speed">Animation playback speed</param>
        public void PlayAnimation(string animationName, AnimationFlags flags = 0, int slot = 0, int layer = 0, float blend = 0.175f, float speed = 1.0f)
        {
            NativeEntityMethods.PlayAnimation(this.GetIEntity(), animationName, slot, layer, blend, speed, flags);
        }

        /// <summary>
        /// Stops the currently playing animation.
        /// </summary>
        /// <param name="slot">The character slot.</param>
        /// <param name="layer">The animation layer which we want to stop. If -1, stops all layers.</param>
        /// <param name="blendOutTime"></param>
        public void StopAnimation(int slot = 0, int layer = 0, float blendOutTime = 0)
        {
            if (layer == -1)
                NativeEntityMethods.StopAnimationsInAllLayers(this.GetIEntity(), slot);
            else
                NativeEntityMethods.StopAnimationInLayer(this.GetIEntity(), slot, layer, blendOutTime);
        }

        /// <summary>
        /// Frees the specified slot of all objects.
        /// </summary>
        /// <param name="slot"></param>
        public void FreeSlot(int slot)
        {
            NativeEntityMethods.FreeSlot(this.GetIEntity(), slot);
        }

        /// <summary>
        /// Requests movement at the specified slot, providing an animated character is currently loaded.
        /// </summary>
        /// <param name="request"></param>
        public void AddMovement(ref EntityMovementRequest request)
        {
            NativeEntityMethods.AddMovement(this.GetIAnimatedCharacter(), ref request);
        }

        /// <summary>
        /// Gets the absolute of the specified joint
        /// </summary>
        /// <param name="jointName">Name of the joint</param>
        /// <param name="characterSlot">Slot containing the character</param>
        /// <returns>Absolute of the specified joint</returns>
        public QuatT GetJointAbsolute(string jointName, int characterSlot = 0)
        {
            return NativeEntityMethods.GetJointAbsolute(this.GetIEntity(), jointName, characterSlot);
        }

        /// <summary>
        /// Gets the default absolute of the specified joint
        /// </summary>
        /// <param name="jointName">Name of the joint</param>
        /// <param name="characterSlot">Slot containing the character</param>
        /// <returns>Default absolute of the specified joint</returns>
        public QuatT GetJointAbsoluteDefault(string jointName, int characterSlot = 0)
        {
            return NativeEntityMethods.GetJointAbsoluteDefault(this.GetIEntity(), jointName, characterSlot);
        }

        /// <summary>
        /// Sets the absolute of the specified joint
        /// </summary>
        /// <param name="jointName">Name of the joint</param>
        /// <param name="absolute">New absolute</param>
        /// <param name="characterSlot">Slot containing the character</param>
        public void SetJointAbsolute(string jointName, QuatT absolute, int characterSlot = 0)
        {
            NativeEntityMethods.SetJointAbsolute(this.GetIEntity(), jointName, characterSlot, absolute);
        }

        /// <summary>
        /// Gets the relative of the specified joint
        /// </summary>
        /// <param name="jointName">Name of the joint</param>
        /// <param name="characterSlot">Slot containing the character</param>
        /// <returns>Relative of the specified joint</returns>
        public QuatT GetJointRelative(string jointName, int characterSlot = 0)
        {
            return NativeEntityMethods.GetJointRelative(this.GetIEntity(), jointName, characterSlot);
        }

        /// <summary>
        /// Gets the default relative of the specified joint
        /// </summary>
        /// <param name="jointName">Name of the joint</param>
        /// <param name="characterSlot">Slot containing the character</param>
        /// <returns>Default relative of the specified joint</returns>
        public QuatT GetJointRelativeDefault(string jointName, int characterSlot = 0)
        {
            return NativeEntityMethods.GetJointRelativeDefault(this.GetIEntity(), jointName, characterSlot);
        }

        public ParticleEmitter LoadParticleEmitter(ParticleEffect particleEffect, ref ParticleSpawnParameters spawnParams, int slot = -1)
        {
            var ptr = NativeEntityMethods.LoadParticleEmitter(this.GetIEntity(), slot, particleEffect.Handle, ref spawnParams);
            if (ptr != IntPtr.Zero)
                return new ParticleEmitter(ptr);

            return null;
        }
    }
}