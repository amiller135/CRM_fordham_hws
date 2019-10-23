import numpy as np
import pandas as pd

# ------------------------- Load/create transition matrices ----------------------#
transition_1y = pd.read_csv("/Users/francescao/Credit_risk_mgt/transition_1year.csv")
transition_3y = pd.read_csv("/Users/francescao/Credit_risk_mgt/transition_3year.csv")
transition_5y = pd.read_csv("/Users/francescao/Credit_risk_mgt/transition_5year.csv")
transition_1y.index = ['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C']
transition_3y.index = ['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C']
transition_5y.index = ['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C']


transition_1y.drop(columns='One Year',inplace=True)
# .index.name='One Year'
transition_1y.index.name='One Year'

transition_3y.drop(columns='three year',inplace=True)
# .index.name='One Year'
transition_3y.index.name='three year'

transition_5y.drop(columns='five year',inplace=True)
# .index.name='One Year'
transition_5y.index.name='five year'

columns = ['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C']

transition_2y = pd.DataFrame(index=['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C'])
transition_2y.index.name = 'two year'
for col in columns:
    transition_2y[col] = np.array(transition_1y[col] + (transition_3y[col] - transition_1y[col])/(3-1) * (2-1))




transition_4y = pd.DataFrame(index=['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C'])
transition_4y.index.name = 'four year'

for col in columns:
    transition_4y[col] = np.array(transition_3y[col] + (transition_5y[col] - transition_3y[col])/(5-3) * (5-4))

transitions_matrices = [transition_1y, transition_2y, transition_3y, transition_4y, transition_5y]

# ------------ define function to find risk_neutral transitions --------------#


def solve_pi(spread, rf_rates, lst_transitions, recovery_rate=0.60, rating='AA'):
    '''
    :param spread: numpy_array
    :param rf_rates: numpy_array
    :param lst_transitions: lst_transition matrix dataframes
    :param recovery_rate: float
    :param rating: string one of the following ['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C']
    :return: risk neutral transition vector for given rating
    '''

    lst_index = np.array(['AAA', 'AA', 'A', 'BBB', 'BB', 'B', 'CCC/C'])
    inv_rr = 1 - recovery_rate
    rf_bond = rf_rates + 1
    risky_bond = 1/(rf_rates + spread + 1)
    risk_neutral_transition = []
    pi_values = []
    for num in range(5):
        state_vector = np.array(lst_transitions[num].loc[rating].values)
        state_vector = state_vector/100
        index = np.where(lst_index == rating)[0][0]
        state_vector[index] = -(1 - state_vector[index])
        D = - state_vector.sum()
        pi = (1-rf_bond[num]*risky_bond[num]) / (inv_rr * D)
        rn = state_vector*pi
        risk_neutral_transition.append(rn)
        pi_values.append(pi)
    for item in risk_neutral_transition:
        item[index] = item[index] + 1
    return risk_neutral_transition, pi_values


if __name__ == "__main__":
    # ---------------------- Parameters for test run ----------------------------#
    spread1 = np.array([0.01153615, 0.01221374, 0.00913923, 0.00937356, 0.00975359])
    rf = np.array([0.0159, 0.0162, 0.0159, 0.016, 0.0161])
    rn_trans, pis = solve_pi(spread=spread1, rf_rates=rf, lst_transitions=transitions_matrices, rating='A')




