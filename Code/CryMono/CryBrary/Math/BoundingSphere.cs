﻿/*
* Copyright (c) 2007-2010 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
using System;
using System.Globalization;
using System.Runtime.InteropServices;

namespace CryEngine
{
    /// <summary>
    /// Represents a bounding sphere in three dimensional space.
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct BoundingSphere : IEquatable<BoundingSphere>, IFormattable
    {
        /// <summary>
        /// The center of the sphere in three dimensional space.
        /// </summary>
        public Vec3 Center;

        /// <summary>
        /// The radious of the sphere.
        /// </summary>
        public float Radius;

        /// <summary>
        /// Initializes a new instance of the <see cref="CryEngine.BoundingBox"/> struct.
        /// </summary>
        /// <param name="center">The center of the sphere in three dimensional space.</param>
        /// <param name="radius">The radius of the sphere.</param>
        public BoundingSphere(Vec3 center, float radius)
        {
            Center = center;
            Radius = radius;
        }

        /// <summary>
        /// Determines if there is an intersection between the current object and a <see cref="CryEngine.Ray"/>.
        /// </summary>
        /// <param name="ray">The ray to test.</param>
        /// <returns>Whether the two objects intersected.</returns>
        public bool Intersects(ref Ray ray)
        {
            float distance;
            return Collision.RayIntersectsSphere(ref ray, ref this, out distance);
        }

        /// <summary>
        /// Determines if there is an intersection between the current object and a <see cref="CryEngine.Ray"/>.
        /// </summary>
        /// <param name="ray">The ray to test.</param>
        /// <param name="distance">When the method completes, contains the distance of the intersection,
        /// or 0 if there was no intersection.</param>
        /// <returns>Whether the two objects intersected.</returns>
        public bool Intersects(ref Ray ray, out float distance)
        {
            return Collision.RayIntersectsSphere(ref ray, ref this, out distance);
        }

        /// <summary>
        /// Determines if there is an intersection between the current object and a <see cref="CryEngine.Ray"/>.
        /// </summary>
        /// <param name="ray">The ray to test.</param>
        /// <param name="point">When the method completes, contains the point of intersection,
        /// or <see cref="CryEngine.Vec3"/> if there was no intersection.</param>
        /// <returns>Whether the two objects intersected.</returns>
        public bool Intersects(ref Ray ray, out Vec3 point)
        {
            return Collision.RayIntersectsSphere(ref ray, ref this, out point);
        }

        /// <summary>
        /// Determines if there is an intersection between the current object and a <see cref="CryEngine.Plane"/>.
        /// </summary>
        /// <param name="plane">The plane to test.</param>
        /// <returns>Whether the two objects intersected.</returns>
        public PlaneIntersectionType Intersects(ref Plane plane)
        {
            return Collision.PlaneIntersectsSphere(ref plane, ref this);
        }

        /// <summary>
        /// Determines if there is an intersection between the current object and a triangle.
        /// </summary>
        /// <param name="vertex1">The first vertex of the triangle to test.</param>
        /// <param name="vertex2">The second vertex of the triagnle to test.</param>
        /// <param name="vertex3">The third vertex of the triangle to test.</param>
        /// <returns>Whether the two objects intersected.</returns>
        public bool Intersects(ref Vec3 vertex1, ref Vec3 vertex2, ref Vec3 vertex3)
        {
            return Collision.SphereIntersectsTriangle(ref this, ref vertex1, ref vertex2, ref vertex3);
        }

        /// <summary>
        /// Determines if there is an intersection between the current object and a <see cref="CryEngine.BoundingBox"/>.
        /// </summary>
        /// <param name="box">The box to test.</param>
        /// <returns>Whether the two objects intersected.</returns>
        public bool Intersects(ref BoundingBox box)
        {
            return Collision.BoxIntersectsSphere(ref box, ref this);
        }

        /// <summary>
        /// Determines if there is an intersection between the current object and a <see cref="CryEngine.BoundingSphere"/>.
        /// </summary>
        /// <param name="sphere">The sphere to test.</param>
        /// <returns>Whether the two objects intersected.</returns>
        public bool Intersects(ref BoundingSphere sphere)
        {
            return Collision.SphereIntersectsSphere(ref this, ref sphere);
        }

        /// <summary>
        /// Determines whether the current objects contains a point.
        /// </summary>
        /// <param name="point">The point to test.</param>
        /// <returns>The type of containment the two objects have.</returns>
        public bool Contains(ref Vec3 point)
        {
            return Collision.SphereContainsPoint(ref this, ref point);
        }

        /// <summary>
        /// Determines whether the current objects contains a triangle.
        /// </summary>
        /// <param name="vertex1">The first vertex of the triangle to test.</param>
        /// <param name="vertex2">The second vertex of the triagnle to test.</param>
        /// <param name="vertex3">The third vertex of the triangle to test.</param>
        /// <returns>The type of containment the two objects have.</returns>
        public ContainmentType Contains(ref Vec3 vertex1, ref Vec3 vertex2, ref Vec3 vertex3)
        {
            return Collision.SphereContainsTriangle(ref this, ref vertex1, ref vertex2, ref vertex3);
        }

        /// <summary>
        /// Determines whether the current objects contains a <see cref="CryEngine.BoundingBox"/>.
        /// </summary>
        /// <param name="box">The box to test.</param>
        /// <returns>The type of containment the two objects have.</returns>
        public ContainmentType Contains(ref BoundingBox box)
        {
            return Collision.SphereContainsBox(ref this, ref box);
        }

        /// <summary>
        /// Determines whether the current objects contains a <see cref="CryEngine.BoundingSphere"/>.
        /// </summary>
        /// <param name="sphere">The sphere to test.</param>
        /// <returns>The type of containment the two objects have.</returns>
        public ContainmentType Contains(ref BoundingSphere sphere)
        {
            return Collision.SphereContainsSphere(ref this, ref sphere);
        }

        /// <summary>
        /// Constructs a <see cref="CryEngine.BoundingSphere"/> that fully contains the given points.
        /// </summary>
        /// <param name="points">The points that will be contained by the sphere.</param>
        /// <param name="result">When the method completes, contains the newly constructed bounding sphere.</param>
        public static void FromPoints(Vec3[] points, out BoundingSphere result)
        {
            // Find the center of all points.
            Vec3 center = Vec3.Zero;
            for (int i = 0; i < points.Length; ++i)
            {
                center += points[i];
            }

            // This is the center of our sphere.
            center /= (float)points.Length;

            // Find the radius of the sphere
            float radius = 0f;
            for (int i = 0; i < points.Length; ++i)
            {
                // We are doing a relative distance comparasin to find the maximum distance
                // from the center of our sphere.
                float distance = center.GetDistanceSquared(points[i]);

                if (distance > radius)
                    radius = distance;
            }

            // Find the real distance from the DistanceSquared.
            radius = (float)Math.Sqrt(radius);

            // Construct the sphere.
            result.Center = center;
            result.Radius = radius;
        }

        /// <summary>
        /// Constructs a <see cref="CryEngine.BoundingSphere"/> that fully contains the given points.
        /// </summary>
        /// <param name="points">The points that will be contained by the sphere.</param>
        /// <returns>The newly constructed bounding sphere.</returns>
        public static BoundingSphere FromPoints(Vec3[] points)
        {
            BoundingSphere result;
            FromPoints(points, out result);
            return result;
        }

        /// <summary>
        /// Constructs a <see cref="CryEngine.BoundingSphere"/> from a given box.
        /// </summary>
        /// <param name="box">The box that will designate the extents of the sphere.</param>
        /// <param name="result">When the method completes, the newly constructed bounding sphere.</param>
        public static void FromBox(ref BoundingBox box, out BoundingSphere result)
        {
            result.Center = Vec3.CreateLerp(box.Minimum, box.Maximum, 0.5f);

            float x = box.Minimum.X - box.Maximum.X;
            float y = box.Minimum.Y - box.Maximum.Y;
            float z = box.Minimum.Z - box.Maximum.Z;

            var distance = (Math.Sqrt((x * x) + (y * y) + (z * z)));
            result.Radius = (float)distance * 0.5f;
        }

        /// <summary>
        /// Constructs a <see cref="CryEngine.BoundingSphere"/> from a given box.
        /// </summary>
        /// <param name="box">The box that will designate the extents of the sphere.</param>
        /// <returns>The newly constructed bounding sphere.</returns>
        public static BoundingSphere FromBox(BoundingBox box)
        {
            BoundingSphere result;
            FromBox(ref box, out result);
            return result;
        }

        /// <summary>
        /// Constructs a <see cref="CryEngine.BoundingSphere"/> that is the as large as the total combined area of the two specified spheres.
        /// </summary>
        /// <param name="value1">The first sphere to merge.</param>
        /// <param name="value2">The second sphere to merge.</param>
        /// <param name="result">When the method completes, contains the newly constructed bounding sphere.</param>
        public static void Merge(ref BoundingSphere value1, ref BoundingSphere value2, out BoundingSphere result)
        {
            Vec3 difference = value2.Center - value1.Center;

            float length = difference.Length;
            float radius = value1.Radius;
            float radius2 = value2.Radius;

            if (radius + radius2 >= length)
            {
                if (radius - radius2 >= length)
                {
                    result = value1;
                    return;
                }

                if (radius2 - radius >= length)
                {
                    result = value2;
                    return;
                }
            }

            Vec3 vector = difference * (1.0f / length);
            float min = MathHelpers.Min(-radius, length - radius2);
            float max = (MathHelpers.Max(radius, length + radius2) - min) * 0.5f;

            result.Center = value1.Center + vector * (max + min);
            result.Radius = max;
        }

        /// <summary>
        /// Constructs a <see cref="CryEngine.BoundingSphere"/> that is the as large as the total combined area of the two specified spheres.
        /// </summary>
        /// <param name="value1">The first sphere to merge.</param>
        /// <param name="value2">The second sphere to merge.</param>
        /// <returns>The newly constructed bounding sphere.</returns>
        public static BoundingSphere Merge(BoundingSphere value1, BoundingSphere value2)
        {
            BoundingSphere result;
            Merge(ref value1, ref value2, out result);
            return result;
        }

        /// <summary>
        /// Tests for equality between two objects.
        /// </summary>
        /// <param name="left">The first value to compare.</param>
        /// <param name="right">The second value to compare.</param>
        /// <returns><c>true</c> if <paramref name="left"/> has the same value as <paramref name="right"/>; otherwise, <c>false</c>.</returns>
        public static bool operator ==(BoundingSphere left, BoundingSphere right)
        {
            return left.Equals(right);
        }

        /// <summary>
        /// Tests for inequality between two objects.
        /// </summary>
        /// <param name="left">The first value to compare.</param>
        /// <param name="right">The second value to compare.</param>
        /// <returns><c>true</c> if <paramref name="left"/> has a different value than <paramref name="right"/>; otherwise, <c>false</c>.</returns>
        public static bool operator !=(BoundingSphere left, BoundingSphere right)
        {
            return !left.Equals(right);
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return string.Format(CultureInfo.CurrentCulture, "Center:{0} Radius:{1}", Center.ToString(), Radius.ToString());
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <param name="format">The format.</param>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public string ToString(string format)
        {
            if (format == null)
                return ToString();

            return string.Format(CultureInfo.CurrentCulture, "Center:{0} Radius:{1}", Center.ToString(), Radius.ToString());
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <param name="formatProvider">The format provider.</param>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public string ToString(IFormatProvider formatProvider)
        {
            return string.Format(formatProvider, "Center:{0} Radius:{1}", Center.ToString(), Radius.ToString());
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <param name="format">The format.</param>
        /// <param name="formatProvider">The format provider.</param>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public string ToString(string format, IFormatProvider formatProvider)
        {
            if (format == null)
                return ToString(formatProvider);

            return string.Format(formatProvider, "Center:{0} Radius:{1}", Center.ToString(), Radius.ToString());
        }

        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        /// <returns>
        /// A hash code for this instance, suitable for use in hashing algorithms and data structures like a hash table. 
        /// </returns>
        public override int GetHashCode()
        {
            // Overflow is fine, just wrap
            unchecked
            {
                int hash = 17;

                hash = hash * 29 + Center.GetHashCode();
                hash = hash * 29 + Radius.GetHashCode();

                return hash;
            }
        }

        /// <summary>
        /// Determines whether the specified <see cref="CryEngine.Vec4"/> is equal to this instance.
        /// </summary>
        /// <param name="value">The <see cref="CryEngine.Vec4"/> to compare with this instance.</param>
        /// <returns>
        /// <c>true</c> if the specified <see cref="CryEngine.Vec4"/> is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        public bool Equals(BoundingSphere value)
        {
            return Center == value.Center && Radius == value.Radius;
        }

        /// <summary>
        /// Determines whether the specified <see cref="System.Object"/> is equal to this instance.
        /// </summary>
        /// <param name="value">The <see cref="System.Object"/> to compare with this instance.</param>
        /// <returns>
        /// <c>true</c> if the specified <see cref="System.Object"/> is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        public override bool Equals(object value)
        {
            if (value == null)
                return false;

            if (value.GetType() != GetType())
                return false;

            return Equals((BoundingSphere)value);
        }

#if SlimDX1xInterop
        /// <summary>
        /// Performs an implicit conversion from <see cref="CryEngine.BoundingSphere"/> to <see cref="SlimDX.BoundingSphere"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static implicit operator SlimDX.BoundingSphere(BoundingSphere value)
        {
            return new SlimDX.BoundingSphere(value.Center, value.Radius);
        }

        /// <summary>
        /// Performs an implicit conversion from <see cref="SlimDX.BoundingSphere"/> to <see cref="CryEngine.BoundingSphere"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static implicit operator BoundingSphere(SlimDX.BoundingSphere value)
        {
            return new BoundingSphere(value.Center, value.Radius);
        }
#endif

#if SlimDX1xInterop
        /// <summary>
        /// Performs an implicit conversion from <see cref="CryEngine.BoundingSphere"/> to <see cref="Microsoft.Xna.Framework.BoundingSphere"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static implicit operator Microsoft.Xna.Framework.BoundingSphere(BoundingSphere value)
        {
            return new Microsoft.Xna.Framework.BoundingSphere(value.Center, value.Radius);
        }

        /// <summary>
        /// Performs an implicit conversion from <see cref="Microsoft.Xna.Framework.BoundingSphere"/> to <see cref="CryEngine.BoundingSphere"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static implicit operator BoundingSphere(Microsoft.Xna.Framework.BoundingSphere value)
        {
            return new BoundingSphere(value.Center, value.Radius);
        }
#endif
    }
}