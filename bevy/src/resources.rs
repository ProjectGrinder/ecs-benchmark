use bevy::prelude::*;

#[derive(Resource)]
pub struct SimulationConfig {
    pub max_entities: usize,
    pub repetitions: usize,
    pub strange_ratio: f32,
    pub entity_rate: usize
}

#[derive(Resource)]
pub struct PotatoEntities(Vec<Entity>);