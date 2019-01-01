import random
import logging
import copy
from . import key_profiles
from . import key_transitions

class Evolver:
    def __init__(self, generator):
        self.logger = logging.getLogger('evolver')
        self.generator = generator
        self.logger.info('Evolver() <- generator={}'.format(generator))

    def evolve_key_profiles(self, kps, retain=0.2, random_retain=0.2, crossover=0.3, mutation_prob=0.2, mutation_ratio=0.1):
        ''' Evolver, assumes key_profiles sorted by best performance first '''
        self.logger.info('Start evolve_key_profiles() <- kps={}, retain={}, random_retain={}, crossover={}, mutation_prob={}, mutation_ratio={}'.format(kps, retain, random_retain, crossover, mutation_prob, mutation_ratio))
        # Number of key profiles from the old population that we are keeping
        retain_length = int(len(kps) * retain)
        self.logger.debug('retain_length:{}'.format(retain_length))
        # Other random key profiles from the population that we are keeping
        random_retain_length = int(len(kps) * random_retain)
        self.logger.debug('random_retain_length:{}'.format(random_retain_length))
        # Number of key profiles that will be generated by mixing
        # the major and minor key profiles of 2 different samples
        crossover_length = int(len(kps) * crossover)
        self.logger.debug('crossover_length:{}'.format(crossover_length))

        # Initialize the parents of the new population
        new_key_profiles = copy.deepcopy(kps[:retain_length])

        # Select random key profiles from the previous generation
        random_old = []
        while len(random_old) < random_retain_length:
            kp = random.choice(kps[retain_length:])
            if kp not in random_old and kp not in new_key_profiles:
                self.logger.debug('random_kp_retained:{}'.format(kp))
                random_old.append(copy.deepcopy(kp))
        new_key_profiles.extend(random_old)

        # Crossover parents to fill some of the population
        children = []
        while len(children) < crossover_length:
            male_name = random.choice(new_key_profiles)
            female_name = random.choice(new_key_profiles)
            if male_name != female_name:
                # Child is made of the major key of
                # the male and minor key of the female
                child_name = '({},{})'.format(male_name, female_name)
                male_kp = key_profiles.get(male_name)
                female_kp = key_profiles.get(female_name)
                child_kp = male_kp[:12] + female_kp[12:]
                self.logger.debug('child:{}'.format(child_name))
                key_profiles.insert_new(child_name, child_kp)
                children.append(child_name)
        new_key_profiles.extend(children)

        # Do some mutations. Mutations consist in taking a 'fraction'
        # of the magnitude of a pitch class and give it to another pitch class
        # The fraction is defined by 'mutation_ratio', default is 10%
        for idx, key_profile_name in enumerate(new_key_profiles):
            if mutation_prob > random.random():
                mutation_name = '{}*'.format(key_profile_name)
                self.logger.debug('mutation:{}'.format(mutation_name))
                kp = copy.deepcopy(key_profiles.get(key_profile_name))
                index_range = random.choice([(0, 11), (12, 23)])
                pc1_index = 0
                pc2_index = 0
                while pc1_index == pc2_index:
                    pc1_index = random.randint(index_range[0], index_range[1])
                    pc2_index = random.randint(index_range[0], index_range[1])
                delta = kp[pc1_index] * mutation_ratio
                self.logger.debug('pc1 {} changes by {}, from {} to {}'.format(pc1_index, delta, kp[pc1_index], kp[pc1_index] - delta))
                self.logger.debug('pc2 {} changes by {}, from {} to {}'.format(pc2_index, delta, kp[pc2_index], kp[pc2_index] + delta))
                kp[pc1_index] -= delta
                kp[pc2_index] += delta
                key_profiles.insert_new(mutation_name, kp)
                new_key_profiles[idx] = mutation_name

        # Fill the rest of the population with random key profiles
        while len(kps) > len(new_key_profiles):
            kp_name = self.generator.generate_key_profile()
            self.logger.debug('Generated key_profile: {}'.format(kp_name))
            new_key_profiles.append(kp_name)
        self.logger.info('Done evevolve_key_profilesolve() -> new_key_profiles={}'.format(new_key_profiles))
        return new_key_profiles

    def evolve_key_transitions(self, kts, retain=0.2, random_retain=0.2, mutation_prob=0.2, mutation_ratio=0.1):
        ''' Evolver, assumes key_transitions sorted by best performance first'''
        self.logger.info('Start evolve_key_transitions() <- kts={}, retain={}, random_retain={}, mutation_prob={}, mutation_ratio={}'.format(kts, retain, random_retain, mutation_prob, mutation_ratio))

        # Number of key transitions from the old population that we are keeping
        retain_length = int(len(kts) * retain)
        self.logger.debug('retain_length:{}'.format(retain_length))
        # Other random key transitions from the population that we are keeping
        random_retain_length = int(len(kts) * random_retain)
        self.logger.debug('random_retain_length:{}'.format(random_retain_length))

        # Initialize the parents of the new population
        new_key_transitions = copy.deepcopy(kts[:retain_length])

        # Select random key transitions from the previous generation
        random_old = []
        while len(random_old) < random_retain_length:
            kt = random.choice(kts[retain_length:])
            if kt not in random_old and kt not in new_key_transitions:
                self.logger.debug('random_kt_retained:{}'.format(kt))
                random_old.append(copy.deepcopy(kt))
        new_key_transitions.extend(random_old)

        # Do some mutations.
        for idx, key_transition_name in enumerate(new_key_transitions):
            if key_transition_name.startswith('ktg'):
                if mutation_prob > random.random():
                    kt = copy.deepcopy(key_transitions.get(key_transition_name))
                    ratio = kt[13] # Yes, 13 is a magic number, 20 also works
                    new_ratio = random.randrange(ratio - ratio*mutation_ratio, ratio + ratio*mutation_ratio)
                    kt = self.generator.generate_geometric_key_transition(new_ratio)
                    mutation_name = '{}*'.format(key_transition_name)
                    self.logger.debug('mutation:{}'.format(mutation_name))
                    key_transitions.insert_new(mutation_name, kt)
                    new_key_transitions[idx] = mutation_name
            elif key_transition_name.startswith('kts'):
                if mutation_prob > random.random():
                    mutation_name = '{}*'.format(key_transition_name)
                    self.logger.debug('mutation:{}'.format(mutation_name))
                    kt = copy.deepcopy(key_transitions.get(key_transition_name))
                    key1_index = 0
                    key2_index = 0
                    while key1_index == key2_index:
                        key1_index = random.randint(0, 23)
                        key2_index = random.randint(0, 23)
                    delta = kt[key1_index] * mutation_ratio
                    self.logger.debug('key1 {} changes by {}, from {} to {}'.format(key1_index, delta, kt[key1_index], kt[key1_index] - delta))
                    self.logger.debug('key2 {} changes by {}, from {} to {}'.format(key2_index, delta, kt[key2_index], kt[key2_index] + delta))
                    kt[key1_index] -= delta
                    kt[key2_index] += delta
                    key_transitions.insert_new(mutation_name, kt)
                    new_key_transitions[idx] = mutation_name

        # Fill the rest of the population with random key transitions
        while len(kts) > len(new_key_transitions):
            kt_name = self.generator.generate_key_transition()
            self.logger.debug('Generated key_transition: {}'.format(kt_name))
            new_key_transitions.append(kt_name)
        self.logger.info('Done evolve_key_transitions() -> new_key_transitions={}'.format(new_key_transitions))
        return new_key_transitions

