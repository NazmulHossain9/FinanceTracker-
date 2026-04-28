# Models

Plain data-holder classes. No Qt parent hierarchy, no database access.

## Transaction

Represents a single financial event.

| Field | Type | Notes |
|---|---|---|
| `id` | `int` | `-1` for unsaved records; assigned by DB on insert |
| `title` | `QString` | Free-form label |
| `amount` | `double` | Always positive; sign is implied by `type` |
| `type` | `TransactionType` | `INCOME` or `EXPENSE` |
| `category` | `QString` | User-defined string |
| `date` | `QDate` | |

Two constructors: one for new (no `id`), one for records loaded from DB (with `id`). Use `formattedAmount()` for display — it prepends `+`/`-` and formats to 2 decimal places.

## Account

Represents a financial account (checking, savings, credit).

`deposit()` and `withdraw()` mutate `m_balance` in-place. Not currently persisted to the database — extend `DatabaseManager` if persistence is needed.

## Budget

Maps category names to spending limits. No identity (`id`) field — treated as an in-memory policy object.

`isOverBudget(category, spent)` returns `true` when `spent > getLimit(category)`. Returns `false` if no limit is set for the category.
