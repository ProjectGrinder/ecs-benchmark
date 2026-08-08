use bevy::prelude::*;

#[derive(Component, Debug, Clone, Copy)]
pub struct Position(pub Vec2);

#[derive(Component, Debug, Clone, Copy)]
pub struct Velocity(pub Vec2);

#[derive(Component, Debug, Clone, Copy)]
pub struct Strangeness;

#[derive(Component, Debug, Clone, Copy)]
pub struct Potato {
    pub count: i32
}

#[derive(Component, Debug, Clone, Copy)]
pub struct Target {
    pub entity: Entity
}

#[derive(Component, Debug, Clone, Copy)]
pub struct Lifetime {
    pub lifetime: i32
}

#[derive(Component, Debug, Clone, Copy)]
pub struct Step {
    pub sum: i32,
    pub step: i32
}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit0 {}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit1 {}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit2 {}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit3 {}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit4 {}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit5 {}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit6 {}

#[derive(Component, Debug, Clone, Copy)]
pub struct Bit7 {}