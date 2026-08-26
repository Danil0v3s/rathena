// Aggregate the NPCs in this subtree. Each NPC file `export const`s
// a Registration record (no side-effect register*() call inside the
// file); this index imports them and makes a single varargs
// register*() call. Same idiomatic shape as Prometheus.

import { tsGuide } from "./kafra";
import { mailbox } from "./mailbox";
import { tsLab } from "./tslab";
// Smoke test for the declarative registrars. Left OUT of the default bundle
// because it places real objects in the world (shops + warps in prontera,
// porings and a `noteleport` mapflag on prt_fild08). Uncomment, rebuild, and
// watch the "[ts-scripting] materialized:" line at boot to exercise them:
// import "./registrar_smoketest";

registerNpc(tsGuide, mailbox, tsLab);
