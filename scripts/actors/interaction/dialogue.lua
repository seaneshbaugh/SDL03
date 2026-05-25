function initialize()
  print("interaction/dialogue initialized for " .. actor.name)
end

function on_interact(other_actor)
  mapState:startDialogue(actor.dialogueId)
end
