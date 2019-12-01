ratings = {'aapl': 'AA', 'ge': 'A', 'gs': 'A', 'msft': 'AAA', 'tsla': 'CCC/C'}
dict_pi = {}
for key in ratings.keys():
    rn_trans, pis = solve_pi(spread=spread1, rf_rates=rf, lst_transitions=transitions_matrices, rating=ratings[key])
    dict_pi.update({key: pis})

pds = {'aapl': np.array([0.048907, 0.00201, 0.000232, 0.001417, 0.0043490]),
       'ge': np.array([0.144, 0.390224, 0.258475, 0.388114, 0.358337]),
       'tsla': np.array([0.092848, 0.115642, 0.097056, 0.130158, 0.180231]),
       'msft': np.array([0.0332, 0.0000002846, 0.00000139, 0.00002889, 0.000182]),
       'gs': np.array([0.124359, 0.396413, 0.008159, 0.032644, 0.060846])}
rrs = {'msft': 0.4

       }
solve_spread = {}
for key in pds.keys():
    solve_spread.update({key: pds[key] * dict_pi[key] * np.repeat((1 - 0.4), 5)})

