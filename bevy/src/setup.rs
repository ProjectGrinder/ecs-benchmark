use bevy::prelude::*;
use crate::components::Potato;
use crate::components::Target;
use crate::resources;
use crate::components;
use rand::RngExt;

pub fn test1_setup(mut commands: Commands,
    config: Res<resources::SimulationConfig>, mut rng_res: ResMut<resources::RandomNumberGenerator>) {
    let rng = &mut rng_res.rng;
    for _ in 0..config.max_entities {
        let pos = components::Position(Vec2::new(rng.random_range(-100.0..100.0), rng.random_range(-100.0..100.0)));
        let vel = components::Velocity(Vec2::new(rng.random_range(-1.0..1.0), rng.random_range(-1.0..1.0)));
        commands.spawn((
            pos,
            vel
        ));
    }
}


pub fn test2_setup(mut commands: Commands, config: Res<resources::SimulationConfig>,
    mut rng_res: ResMut<resources::RandomNumberGenerator>) {
    use rand::seq::SliceRandom;

    let rng = &mut rng_res.rng;
    let max = config.max_entities;
    let num_strange = ((max as f32) * config.strange_ratio).floor() as usize;

    // pick `num_strange` unique indices
    let mut indices: Vec<usize> = (0..max).collect();
    indices.shuffle(rng);
    let mut is_strange = vec![false; max];
    for idx in indices.into_iter().take(num_strange) {
        is_strange[idx] = true;
    }

    for i in 0..max {
        let pos = components::Position(Vec2::new(
            rng.random_range(-100.0..100.0),
            rng.random_range(-100.0..100.0),
        ));
        if is_strange[i] {
            commands.spawn((pos, components::Strangeness));
        } else {
            commands.spawn(pos);
        }
    }
}

pub fn potato_setup(mut commands: Commands, config: Res<resources::SimulationConfig>,
    mut rng_res: ResMut<resources::RandomNumberGenerator>) {
    use rand::seq::SliceRandom;

    let rng = &mut rng_res.rng;
    let max = config.max_entities;

    let mut indices: Vec<usize> = (0..max).collect();
    indices.shuffle(rng);

    let mut entities: Vec<Entity> = vec![];
    for _ in 0..max {
        let entity_id = commands.spawn(Potato{count: rng.random_range(10..15)}).id();
        entities.push(entity_id);
    }
    
    for i in 0..max {
        commands.entity(entities[i]).insert(Target{entity: entities[indices[i]]});
    }
}

pub fn test4_setup(mut commands: Commands, config: Res<resources::SimulationConfig>) {
    let max = config.max_entities;

    for i in 0..max {
        commands.spawn(components::Step{sum: 0, step: i as i32});
    }
}
