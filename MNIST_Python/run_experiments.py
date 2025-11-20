"""
Run multiple experiments with different configurations
"""

import csv
from datetime import datetime
from config_manager import load_config, create_config, update_config, save_config
from experiment import run_experiment


def run_grid_search(experiments, base_config=None, save_results=True):
    """
    Run multiple experiments with different configurations
    
    Args:
        experiments: List of dictionaries with experiment parameters
                    Each dict should have 'name' and parameter overrides
        base_config: Base configuration to start from (loads from config.json if None)
        save_results: Whether to save results to CSV file
        
    Returns:
        List of result dictionaries from all experiments
    """
    
    # Load base config if not provided
    if base_config is None:
        base_config = load_config('config.json')
    
    all_results = []
    timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
    
    print(f"\n{'='*80}")
    print(f"Starting Grid Search with {len(experiments)} experiments")
    print(f"{'='*80}\n")
    
    for i, exp_params in enumerate(experiments, 1):
        # Create a copy of base config
        config = {k: v.copy() if isinstance(v, dict) else v for k, v in base_config.items()}
        
        # Get experiment name
        exp_name = exp_params.pop('name', f'exp_{i}')
        
        # Update config with experiment parameters
        config = update_config(config, **exp_params)
        
        # Optionally save this experiment's config
        save_config(config, f'config_{exp_name}.json')
        
        # Run the experiment
        print(f"\n[{i}/{len(experiments)}] Running: {exp_name}")
        results = run_experiment(config, experiment_name=exp_name, verbose=True)
        
        # Add experiment parameters to results
        results['parameters'] = exp_params
        results['parameters']['name'] = exp_name
        
        all_results.append(results)
    
    # Save results to CSV
    if save_results and all_results:
        csv_filename = f'experiment_results_{timestamp}.csv'
        save_results_to_csv(all_results, csv_filename)
        print(f"\nResults saved to: {csv_filename}")
    
    # Print summary
    print_summary(all_results)
    
    return all_results


def save_results_to_csv(results, filename):
    """Save experiment results to CSV file"""
    
    with open(filename, 'w', newline='') as csvfile:
        fieldnames = [
            'experiment_name',
            'hidden_layers',
            'learning_rate',
            'batch_size',
            'momentum',
            'epochs',
            'final_test_accuracy',
            'final_validation_accuracy',
            'best_test_accuracy',
            'best_validation_accuracy',
            'final_loss',
            'min_loss'
        ]
        
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        
        for result in results:
            row = {
                'experiment_name': result['experiment_name'],
                'hidden_layers': str(result['config']['model']['hidden_layers']),
                'learning_rate': result['config']['training']['learning_rate'],
                'batch_size': result['config']['training']['batch_size'],
                'momentum': result['config']['training']['momentum'],
                'epochs': result['config']['training']['epochs'],
                'final_test_accuracy': f"{result['final_test_accuracy']:.2f}",
                'final_validation_accuracy': f"{result['final_validation_accuracy']:.2f}",
                'best_test_accuracy': f"{result['best_test_accuracy']:.2f}",
                'best_validation_accuracy': f"{result['best_validation_accuracy']:.2f}",
                'final_loss': f"{result['final_loss']:.4f}" if result['final_loss'] else 'N/A',
                'min_loss': f"{result['min_loss']:.4f}" if result['min_loss'] else 'N/A'
            }
            writer.writerow(row)


def print_summary(results):
    """Print a summary of all experiment results"""
    
    print(f"\n{'='*80}")
    print(f"EXPERIMENT SUMMARY")
    print(f"{'='*80}")
    
    # Sort by best test accuracy
    sorted_results = sorted(results, key=lambda x: x['best_test_accuracy'], reverse=True)
    
    print(f"\n{'Rank':<6} {'Name':<20} {'Architecture':<20} {'LR':<10} {'BS':<6} {'Best Test Acc':<15}")
    print(f"{'-'*80}")
    
    for i, result in enumerate(sorted_results, 1):
        name = result['experiment_name']
        arch = str(result['config']['model']['hidden_layers'])
        lr = result['config']['training']['learning_rate']
        bs = result['config']['training']['batch_size']
        acc = result['best_test_accuracy']
        
        print(f"{i:<6} {name:<20} {arch:<20} {lr:<10} {bs:<6} {acc:.2f}%")
    
    print(f"\n{'='*80}\n")


# Example usage
if __name__ == "__main__":
    
    # Define experiments - testing one parameter at a time
    experiments = [
        # Baseline
        {'name': 'baseline', 'hidden_layers': [64, 32], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},


        {'name': 'small_9', 'hidden_layers': [9], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},
        {'name': 'small_4', 'hidden_layers': [4], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},
        {'name': 'small_1', 'hidden_layers': [1], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},


        {'name': 'small_9_9', 'hidden_layers': [9, 9], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},
        {'name': 'small_4_4', 'hidden_layers': [4, 4], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},
        {'name': 'small_1_1', 'hidden_layers': [1, 1], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},


        # Test different architectures

        # {'name': 'arch_widest_1', 'hidden_layers': [512, 128, 64], 'learning_rate': 0.001, 'batch_size': 128, 'epochs': 32},
        # {'name': 'arch_widest_2', 'hidden_layers': [512, 128, 64], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},

        # {'name': 'arch_wider_1', 'hidden_layers': [128, 64], 'learning_rate': 0.001, 'batch_size': 128, 'epochs': 32},
        # {'name': 'arch_wider_2', 'hidden_layers': [128, 64], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},

        # {'name': 'arch_deeper_1', 'hidden_layers': [64, 32, 16], 'learning_rate': 0.001, 'batch_size': 128, 'epochs': 32},
        # {'name': 'arch_deeper_2', 'hidden_layers': [64, 32, 16], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},

        # {'name': 'arch_single_1', 'hidden_layers': [128], 'learning_rate': 0.001, 'batch_size': 128, 'epochs': 32},
        # {'name': 'arch_single_2', 'hidden_layers': [128], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 32},
        
        # Test different learning rates
        # {'name': 'lr_high', 'hidden_layers': [64, 32], 'learning_rate': 0.01, 'batch_size': 64, 'epochs': 10},
        # {'name': 'lr_low', 'hidden_layers': [64, 32], 'learning_rate': 0.0001, 'batch_size': 64, 'epochs': 20},
        
        # Test different batch sizes
        # {'name': 'bs_small', 'hidden_layers': [64, 32], 'learning_rate': 0.001, 'batch_size': 32, 'epochs': 10},
        # {'name': 'bs_large', 'hidden_layers': [64, 32], 'learning_rate': 0.001, 'batch_size': 128, 'epochs': 10},
        
        # Test longer training
        # {'name': 'long_training', 'hidden_layers': [64, 32], 'learning_rate': 0.001, 'batch_size': 64, 'epochs': 50},
    ]
    
    # Run all experiments
    results = run_grid_search(experiments)
