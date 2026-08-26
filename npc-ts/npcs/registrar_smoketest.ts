// Smoke test for the declarative registrars (registerWarp / registerMapFlag /
// registerShop / registerSpawn / registerFloatingNpc).
//
// This exists to prove the C++ materialization path end-to-end at boot:
// every entry below should turn into a live world object and report itself
// in the "[ts-scripting] materialized:" line. Coordinates are all in
// prontera / prt_fild08 so they are easy to eyeball in-game.

// ---- warps ----------------------------------------------------------------
// A round trip: prontera south gate -> prt_fild08 and back.
registerWarp(
    {
        from: { map: "prontera", x: 156, y: 22 },
        area: { xs: 2, ys: 2 },
        to: { map: "prt_fild08", x: 170, y: 375 },
    },
    {
        from: { map: "prt_fild08", x: 170, y: 378 },
        area: { xs: 2, ys: 2 },
        to: { map: "prontera", x: 156, y: 26 },
        type: "warp2", // also triggers for hidden players
    },
);

// ---- map flags ------------------------------------------------------------
registerMapFlag(
    { map: "prt_fild08", flag: "nobranch" },
    { map: "prt_fild08", flag: "noteleport" },
    // value-carrying flag: base exp rate at 200%
    { map: "prt_fild08", flag: "bexp", value: "200" },
);

// ---- shops ----------------------------------------------------------------
registerShop(
    {
        kind: "shop",
        name: "TS Bakery#smoke",
        map: "prontera",
        x: 160,
        y: 180,
        sprite: 60,
        items: [
            { itemId: 512, price: 50 },  // Apple
            { itemId: 513, price: 60 },  // Banana
            { itemId: 514, price: 70 },  // Grape
        ],
    },
    {
        kind: "item",
        name: "TS Barter#smoke",
        map: "prontera",
        x: 162,
        y: 180,
        sprite: 60,
        costItem: 512,     // pay with Apples
        discount: true,    // shop-wide: lets the Merchant Discount skill apply
        items: [{ itemId: 501, price: 2 }],
    },
);

// ---- mob spawns -----------------------------------------------------------
registerSpawn({
    map: "prt_fild08",
    area: { x: 0, y: 0, xs: 0, ys: 0 }, // map-wide
    mobId: 1002,                        // Poring
    amount: 5,
    respawn: { baseMs: 5000, jitterMs: 2000 },
    name: "TS Smoke Poring",
});

// ---- floating NPC ---------------------------------------------------------
// No world position; exists purely to own event labels. Note that OnInit /
// OnClock are registered but not yet *fired* by the host — see the comment
// block in registrar_spawn.cpp.
registerFloatingNpc({
    name: "ts_smoke_ticker",
    async onInit(ctx) {
        ctx.world.debugMessage("[ts-scripting] smoke ticker OnInit reached.");
    },
});
