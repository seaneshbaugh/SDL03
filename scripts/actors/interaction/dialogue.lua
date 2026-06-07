function initialize()
  print("interaction/dialogue initialized for " .. actor.name)
end

function on_interact(interactor)
  mapState:queue_command(states.StartDialogueCommand.new(actor.dialogueId))
end
