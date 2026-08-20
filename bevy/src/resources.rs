use bevy::prelude::*;
use rand::rngs::Xoshiro256PlusPlus;

/// Shared deterministic pseudorandom number generator for systems that need randomness.
#[derive(Resource)]
pub struct RandomNumberGenerator {
    pub rng: Xoshiro256PlusPlus,
}

#[derive(Resource)]
pub struct SimulationConfig {
    pub max_entities: usize,
    pub repetitions: usize,
    pub strange_ratio: f32,
    pub entity_rate: usize
}

#[derive(Resource)]
pub struct PotatoEntities(Vec<Entity>);
