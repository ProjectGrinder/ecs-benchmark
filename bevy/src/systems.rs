use crate::{components::*, resources};
use bevy::prelude::*;
use rand::RngExt;

pub fn movement_system(time: Res<Time>, mut query: Query<(&mut Position, &Velocity)>) {
    let dt = time.delta_secs();

    for (mut pos, vel) in &mut query {
        pos.0 += vel.0 * dt;
    }
}

pub fn strange_system(mut query: Query<(&mut Position, &Strangeness)>) {
    for (mut pos, _vel) in &mut query {
        let x = pos.0.x;
        let y = pos.0.y;
        pos.0.y = -1.0 * x;
        pos.0.x = y;
    }
}

pub fn potato_system(mut queries: ParamSet<(Query<(&mut Potato, &Target)>, Query<&mut Potato>)>) {
    let updates: Vec<_> = {
        let mut query = queries.p0();

        query
            .iter_mut()
            .map(|(mut potato, target)| {
                potato.count -= 1;
                target.entity
            })
            .collect()
    };

    let mut potatoes = queries.p1();

    for entity in updates {
        if let Ok(mut potato) = potatoes.get_mut(entity) {
            potato.count += 1;
        }
    }
}

pub fn spawner_system(config: Res<resources::SimulationConfig>, mut commands: Commands, mut rng_res: ResMut<resources::RandomNumberGenerator>) {
    let rng = &mut rng_res.rng;
    for _ in 0..config.entity_rate {
        commands.spawn((
            Position(Vec2::new(
                rng.random_range(-100.0..100.0),
                rng.random_range(-100.0..100.0),
            )),
            Lifetime { lifetime: 2 },
        ));
    }
}

pub fn destructor_system(mut query: Query<(Entity, &Position, &mut Lifetime)>, mut commands: Commands) {
    for (entity, _, mut lifetime) in &mut query {
        lifetime.lifetime = lifetime.lifetime - 1;
        if lifetime.lifetime == 0 {
            commands.entity(entity).despawn();
        }
    }
}

pub fn counting_system(mut query: Query<(Entity, &mut Step)>, mut commands: Commands) {
    for (entity, mut step) in &mut query {
        step.sum = (step.sum + step.step) % 256;
        if step.sum & 1 != 0 {commands.entity(entity).insert_if_new(Bit0{});}
        else {commands.entity(entity).remove::<Bit0>();}
        if step.sum & 2 != 0 {commands.entity(entity).insert_if_new(Bit1{});}
        else {commands.entity(entity).remove::<Bit1>();}
        if step.sum & 4 != 0 {commands.entity(entity).insert_if_new(Bit2{});}
        else {commands.entity(entity).remove::<Bit2>();}
        if step.sum & 8 != 0 {commands.entity(entity).insert_if_new(Bit3{});}
        else {commands.entity(entity).remove::<Bit3>();}
        if step.sum & 16 != 0 {commands.entity(entity).insert_if_new(Bit4{});}
        else {commands.entity(entity).remove::<Bit4>();}
        if step.sum & 32 != 0 {commands.entity(entity).insert_if_new(Bit5{});}
        else {commands.entity(entity).remove::<Bit5>();}
        if step.sum & 64 != 0 {commands.entity(entity).insert_if_new(Bit6{});}
        else {commands.entity(entity).remove::<Bit6>();}
        if step.sum & 128 != 0 {commands.entity(entity).insert_if_new(Bit7{});}
        else {commands.entity(entity).remove::<Bit7>();}
    }
}

pub fn counting_system_separated_add(mut query: Query<&mut Step>) {
    for mut step in &mut query {
        step.sum = (step.sum + step.step) % 256;
    }
}

pub fn counting_system_separated_0(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 1 != 0 {commands.entity(entity).insert_if_new(Bit0{});}
        else {commands.entity(entity).remove::<Bit0>();}
    }
}

pub fn counting_system_separated_1(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 2 != 0 {commands.entity(entity).insert_if_new(Bit1{});}
        else {commands.entity(entity).remove::<Bit1>();}
    }
}

pub fn counting_system_separated_2(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 4 != 0 {commands.entity(entity).insert_if_new(Bit2{});}
        else {commands.entity(entity).remove::<Bit2>();}
    }
}

pub fn counting_system_separated_3(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 8 != 0 {commands.entity(entity).insert_if_new(Bit3{});}
        else {commands.entity(entity).remove::<Bit3>();}
    }
}

pub fn counting_system_separated_4(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 16 != 0 {commands.entity(entity).insert_if_new(Bit4{});}
        else {commands.entity(entity).remove::<Bit4>();}
    }
}

pub fn counting_system_separated_5(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 32 != 0 {commands.entity(entity).insert_if_new(Bit5{});}
        else {commands.entity(entity).remove::<Bit5>();}
    }
}

pub fn counting_system_separated_6(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 64 != 0 {commands.entity(entity).insert_if_new(Bit6{});}
        else {commands.entity(entity).remove::<Bit6>();}
    }
}

pub fn counting_system_separated_7(query: Query<(Entity, &Step)>, mut commands: Commands) {
    for (entity, step) in &query {
        if step.sum & 128 != 0 {commands.entity(entity).insert_if_new(Bit7{});}
        else {commands.entity(entity).remove::<Bit7>();}
    }
}