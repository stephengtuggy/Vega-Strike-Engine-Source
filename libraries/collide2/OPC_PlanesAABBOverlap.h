/**
 * OPC_PlanesAABBOverlap.h
 *
 * Copyright (C) 2001 Pierre Terdiman
 * Copyright (C) 2001-2025 Daniel Horn, pyramid3d, Stephen G. Tuggy, Benjamen R. Meyer,
 * and other Vega Strike contributors.
 *
 * This file is part of OPCODE - Optimized Collision Detection
 * (http://www.codercorner.com/Opcode.htm) and has been
 * incorporated into Vega Strike
 * (https://github.com/vegastrike/Vega-Strike-Engine-Source).
 *
 * Public Domain
 */
// NO HEADER GUARD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Planes-AABB overlap test.
 *	- original code by Ville Miettinen, from Umbra/dPVS (released on the GD-Algorithms mailing list)
 *	- almost used "as-is", I even left the comments (hence the frustum-related notes)
 *
 *  \param		center			[in] box center
 *  \param		extents			[in] box extents
 *  \param		out_clip_mask	[out] bitmask for active planes
 *  \param		in_clip_mask	[in] bitmask for active planes
 *  \return		TRUE if boxes overlap planes
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline_ bool PlanesCollider::PlanesAABBOverlap(const Point &center,
        const Point &extents,
        uint32_t &out_clip_mask,
        uint32_t in_clip_mask) {
    // Stats
    mNbVolumeBVTests++;

    const Plane *p = mPlanes;

    // Evaluate through all active frustum planes. We determine the relation
    // between the AABB and a plane by using the concept of "near" and "far"
    // vertices originally described by Zhang (and later by Möller). Our
    // variant here uses 3 fabs ops, 6 muls, 7 adds and two floating point
    // comparisons per plane. The routine early-exits if the AABB is found
    // to be outside any of the planes. The loop also constructs a new output
    // clip mask. Most FPUs have a native single-cycle fabsf() operation.

    uint32_t Mask = 1;            // current mask index (1,2,4,8,..)
    uint32_t TmpOutClipMask = 0;            // initialize output clip mask into empty.

    while (Mask <= in_clip_mask)                // keep looping while we have active planes left...
    {
        if (in_clip_mask & Mask)                // if clip plane is active, process it..
        {
            float NP = extents.x * fabsf(p->n.x) + extents.y * fabsf(p->n.y)
                    + extents.z * fabsf(p->n.z);    // ### fabsf could be precomputed
            float MP = center.x * p->n.x + center.y * p->n.y + center.z * p->n.z + p->d;

            if (NP < MP) {                        // near vertex behind the clip plane...
                return FALSE;
            }                // .. so there is no intersection..
            if ((-NP) < MP) {                    // near and far vertices on different sides of plane..
                TmpOutClipMask |= Mask;
            }        // .. so update the clip mask...
        }
        Mask += Mask;                            // mk = (1<<plane)
        p++;                                // advance to next plane
    }

    out_clip_mask = TmpOutClipMask;            // copy output value (temp used to resolve aliasing!)
    return TRUE;                            // indicate that AABB intersects frustum
}
